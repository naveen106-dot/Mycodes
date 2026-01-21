#!/usr/bin/env python3
"""
Smart Parking System for BeagleBone Black
Complete implementation with sensor interfacing, LCD display, and embedded logic engine
"""

import time
import threading
import json
import logging
from datetime import datetime, timedelta
from collections import defaultdict
from enum import Enum
from dataclasses import dataclass, asdict
from typing import Dict, List, Optional, Tuple
import hashlib
import random

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)


class VehicleState(Enum):
    """Vehicle entry/exit states"""
    ABSENT = "absent"
    ENTERING = "entering"
    PARKED = "parked"
    EXITING = "exiting"


@dataclass
class ParkingSlot:
    """Represents a single parking slot"""
    slot_id: int
    is_occupied: bool = False
    vehicle_rfid: Optional[str] = None
    entry_time: Optional[datetime] = None
    last_ir_trigger: Optional[datetime] = None
    tamper_count: int = 0
    is_tampered: bool = False

    def to_dict(self):
        return {
            'slot_id': self.slot_id,
            'is_occupied': self.is_occupied,
            'vehicle_rfid': self.vehicle_rfid,
            'entry_time': self.entry_time.isoformat() if self.entry_time else None,
            'is_tampered': self.is_tampered,
            'tamper_count': self.tamper_count
        }


class IRSensor:
    """Simulates IR sensor with debouncing"""
    def __init__(self, sensor_id: int, gpio_pin: int):
        self.sensor_id = sensor_id
        self.gpio_pin = gpio_pin
        self.is_triggered = False
        self.last_trigger_time = None
        self.debounce_interval = 0.5  # seconds
        self.trigger_count = 0

    def read(self) -> bool:
        """Read IR sensor with debouncing"""
        current_time = datetime.now()
        
        if self.last_trigger_time:
            elapsed = (current_time - self.last_trigger_time).total_seconds()
            if elapsed < self.debounce_interval:
                return False
        
        return self.is_triggered

    def trigger(self):
        """Simulate sensor trigger"""
        self.is_triggered = True
        self.last_trigger_time = datetime.now()
        self.trigger_count += 1
        logger.info(f"IR Sensor {self.sensor_id} triggered (Count: {self.trigger_count})")

    def reset(self):
        """Reset sensor state"""
        self.is_triggered = False


class RFIDReader:
    """Simulates RFID reader with SPI communication"""
    def __init__(self, spi_port: int = 0, chip_select: int = 0):
        self.spi_port = spi_port
        self.chip_select = chip_select
        self.last_read_time = None
        self.read_cooldown = 0.3  # seconds - prevent rapid duplicate reads
        self.read_count = 0
        self.error_count = 0

    def read_rfid(self) -> Optional[str]:
        """Read RFID tag with cooldown to prevent duplicates"""
        current_time = datetime.now()
        
        if self.last_read_time:
            elapsed = (current_time - self.last_read_time).total_seconds()
            if elapsed < self.read_cooldown:
                return None
        
        self.last_read_time = current_time
        self.read_count += 1
        return None  # Return None if no tag detected

    def simulate_read(self, rfid_tag: str) -> str:
        """Simulate reading an RFID tag (for testing)"""
        current_time = datetime.now()
        
        if self.last_read_time:
            elapsed = (current_time - self.last_read_time).total_seconds()
            if elapsed < self.read_cooldown:
                logger.warning(f"RFID read too soon - cooldown active")
                return None
        
        self.last_read_time = current_time
        self.read_count += 1
        logger.info(f"RFID Read: {rfid_tag}")
        return rfid_tag

    def validate_rfid(self, rfid: str) -> bool:
        """Validate RFID format"""
        return len(rfid) >= 8 and rfid.isalnum()


class LCDDisplay:
    """Simulates I2C LCD display (16x2)"""
    def __init__(self, i2c_address: int = 0x27):
        self.i2c_address = i2c_address
        self.display_buffer = ["", ""]
        self.update_count = 0

    def display_status(self, available_slots: int, total_slots: int):
        """Display parking availability"""
        line1 = f"Slots: {available_slots}/{total_slots}"
        line2 = f"Updated: {datetime.now().strftime('%H:%M:%S')}"
        self.display_buffer = [line1, line2]
        self.update_count += 1
        logger.info(f"LCD Display - {line1} | {line2}")

    def display_message(self, line1: str, line2: str = ""):
        """Display custom message"""
        self.display_buffer = [line1[:16], line2[:16]]
        self.update_count += 1
        logger.info(f"LCD Display - {line1} | {line2}")

    def clear(self):
        """Clear display"""
        self.display_buffer = ["", ""]


class TamperDetectionEngine:
    """Detects tampering with sensors"""
    def __init__(self, threshold: int = 5):
        self.threshold = threshold
        self.anomaly_window = 10  # seconds
        self.slot_events = defaultdict(list)

    def check_tamper(self, slot_id: int, event_type: str) -> Tuple[bool, str]:
        """
        Detect tamper attempts
        Corner cases handled:
        - Rapid IR triggers without RFID
        - Multiple rapid RFID reads
        - IR triggers after vehicle parked
        """
        current_time = datetime.now()
        cutoff_time = current_time - timedelta(seconds=self.anomaly_window)
        
        # Remove old events
        self.slot_events[slot_id] = [
            (ts, et) for ts, et in self.slot_events[slot_id]
            if ts > cutoff_time
        ]
        
        self.slot_events[slot_id].append((current_time, event_type))
        
        events_in_window = self.slot_events[slot_id]
        event_count = len(events_in_window)
        
        # Tamper detection logic
        if event_count > self.threshold:
            return True, f"Excessive events ({event_count}) in {self.anomaly_window}s"
        
        # Check for rapid IR without RFID
        ir_count = sum(1 for _, et in events_in_window if et == "IR")
        rfid_count = sum(1 for _, et in events_in_window if et == "RFID")
        
        if ir_count > 3 and rfid_count == 0:
            return True, "IR activity without RFID detection"
        
        # Check for multiple rapid RFID reads
        rfid_events = [(ts, et) for ts, et in events_in_window if et == "RFID"]
        if len(rfid_events) >= 2:
            time_diff = (rfid_events[-1][0] - rfid_events[0][0]).total_seconds()
            if time_diff < 2:
                return True, f"Multiple RFID reads in {time_diff:.1f}s"
        
        return False, "No tampering detected"


class ParkingLogicEngine:
    """Core embedded logic for parking system"""
    def __init__(self, num_slots: int = 10):
        self.num_slots = num_slots
        self.slots: Dict[int, ParkingSlot] = {
            i: ParkingSlot(slot_id=i) for i in range(num_slots)
        }
        self.vehicle_history = defaultdict(list)
        self.entry_exit_log = []
        self.tamper_engine = TamperDetectionEngine(threshold=5)
        self.lock = threading.Lock()
        
        # Duplicate detection
        self.pending_rfid_reads = {}  # rfid -> timestamp
        self.rfid_duplicate_window = 2.0  # seconds
        
        logger.info(f"Parking Logic Engine initialized with {num_slots} slots")

    def get_available_slots(self) -> int:
        """Get number of available slots"""
        with self.lock:
            return sum(1 for s in self.slots.values() if not s.is_occupied)

    def get_slot_status(self, slot_id: int) -> Optional[ParkingSlot]:
        """Get status of specific slot"""
        with self.lock:
            return self.slots.get(slot_id)

    def get_all_slots_status(self) -> List[ParkingSlot]:
        """Get status of all slots"""
        with self.lock:
            return list(self.slots.values())

    def detect_duplicate_rfid(self, rfid: str) -> Tuple[bool, str]:
        """
        Detect duplicate RFID reads
        Returns: (is_duplicate, reason)
        """
        current_time = datetime.now()
        
        if rfid in self.pending_rfid_reads:
            last_read = self.pending_rfid_reads[rfid]
            time_diff = (current_time - last_read).total_seconds()
            
            if time_diff < self.rfid_duplicate_window:
                return True, f"Duplicate RFID read within {time_diff:.2f}s"
        
        # Clean old pending reads
        for tag in list(self.pending_rfid_reads.keys()):
            if (current_time - self.pending_rfid_reads[tag]).total_seconds() > self.rfid_duplicate_window:
                del self.pending_rfid_reads[tag]
        
        self.pending_rfid_reads[rfid] = current_time
        return False, "No duplicate"

    def vehicle_entry(self, slot_id: int, rfid: str) -> Tuple[bool, str]:
        """
        Process vehicle entry
        Corner cases:
        - Slot already occupied
        - Invalid RFID
        - Duplicate RFID detection
        - Tamper detection
        """
        with self.lock:
            if slot_id < 0 or slot_id >= self.num_slots:
                return False, "Invalid slot ID"
            
            slot = self.slots[slot_id]
            current_time = datetime.now()
            
            # Check if slot is already occupied
            if slot.is_occupied:
                return False, f"Slot {slot_id} already occupied"
            
            # Validate RFID
            if not rfid or len(rfid) < 8:
                return False, "Invalid RFID format"
            
            # Check for duplicate RFID
            is_dup, dup_reason = self.detect_duplicate_rfid(rfid)
            if is_dup:
                return False, f"Duplicate detection: {dup_reason}"
            
            # Check for tampering
            is_tampered, tamper_reason = self.tamper_engine.check_tamper(slot_id, "RFID")
            if is_tampered:
                slot.is_tampered = True
                slot.tamper_count += 1
                return False, f"Tamper detected: {tamper_reason}"
            
            # Update slot
            slot.is_occupied = True
            slot.vehicle_rfid = rfid
            slot.entry_time = current_time
            slot.last_ir_trigger = current_time
            
            # Log entry
            entry_log = {
                'timestamp': current_time.isoformat(),
                'action': 'ENTRY',
                'slot_id': slot_id,
                'rfid': rfid,
                'available_slots': self.get_available_slots()
            }
            self.entry_exit_log.append(entry_log)
            self.vehicle_history[rfid].append(entry_log)
            
            logger.info(f"Vehicle ENTRY - Slot {slot_id}, RFID: {rfid}")
            return True, f"Entry successful - Slot {slot_id}"

    def vehicle_exit(self, slot_id: int, rfid: str) -> Tuple[bool, str]:
        """
        Process vehicle exit
        Corner cases:
        - Slot not occupied
        - RFID mismatch
        - Duplicate exit attempts
        - Parking duration tracking
        """
        with self.lock:
            if slot_id < 0 or slot_id >= self.num_slots:
                return False, "Invalid slot ID"
            
            slot = self.slots[slot_id]
            current_time = datetime.now()
            
            # Check if slot is occupied
            if not slot.is_occupied:
                return False, f"Slot {slot_id} is empty"
            
            # Verify RFID matches
            if slot.vehicle_rfid != rfid:
                return False, "RFID mismatch - vehicle mismatch"
            
            # Calculate parking duration
            if slot.entry_time:
                duration = (current_time - slot.entry_time).total_seconds()
                duration_mins = int(duration / 60)
            else:
                duration_mins = -1
            
            # Clear slot
            slot.is_occupied = False
            slot.vehicle_rfid = None
            slot.entry_time = None
            slot.last_ir_trigger = None
            
            # Log exit
            exit_log = {
                'timestamp': current_time.isoformat(),
                'action': 'EXIT',
                'slot_id': slot_id,
                'rfid': rfid,
                'duration_minutes': duration_mins,
                'available_slots': self.get_available_slots()
            }
            self.entry_exit_log.append(exit_log)
            self.vehicle_history[rfid].append(exit_log)
            
            logger.info(f"Vehicle EXIT - Slot {slot_id}, RFID: {rfid}, Duration: {duration_mins}min")
            return True, f"Exit successful - Duration: {duration_mins} min"

    def ir_trigger(self, slot_id: int) -> Tuple[bool, str]:
        """
        Process IR sensor trigger
        Corner cases:
        - IR without corresponding vehicle
        - Multiple IR triggers in short time
        - IR after vehicle parked
        """
        with self.lock:
            if slot_id < 0 or slot_id >= self.num_slots:
                return False, "Invalid slot ID"
            
            slot = self.slots[slot_id]
            current_time = datetime.now()
            
            # Check tamper
            is_tampered, tamper_reason = self.tamper_engine.check_tamper(slot_id, "IR")
            if is_tampered:
                slot.is_tampered = True
                slot.tamper_count += 1
                return False, f"Tamper detected: {tamper_reason}"
            
            # Update last IR trigger
            slot.last_ir_trigger = current_time
            
            logger.info(f"IR Trigger detected at Slot {slot_id}")
            return True, "IR trigger processed"

    def get_statistics(self) -> Dict:
        """Get system statistics"""
        with self.lock:
            total_entries = sum(1 for log in self.entry_exit_log if log['action'] == 'ENTRY')
            total_exits = sum(1 for log in self.entry_exit_log if log['action'] == 'EXIT')
            tampered_slots = sum(1 for s in self.slots.values() if s.is_tampered)
            total_tampering_attempts = sum(s.tamper_count for s in self.slots.values())
            
            avg_duration = 0
            durations = [log['duration_minutes'] for log in self.entry_exit_log 
                        if log['action'] == 'EXIT' and log['duration_minutes'] > 0]
            if durations:
                avg_duration = sum(durations) / len(durations)
            
            return {
                'timestamp': datetime.now().isoformat(),
                'total_slots': self.num_slots,
                'occupied_slots': self.num_slots - self.get_available_slots(),
                'available_slots': self.get_available_slots(),
                'total_entries': total_entries,
                'total_exits': total_exits,
                'unique_vehicles': len(self.vehicle_history),
                'tampered_slots': tampered_slots,
                'total_tampering_attempts': total_tampering_attempts,
                'average_duration_minutes': round(avg_duration, 2),
                'recent_logs': self.entry_exit_log[-10:]
            }


class SmartParkingSystem:
    """Main system coordinator"""
    def __init__(self, num_slots: int = 10):
        self.logic_engine = ParkingLogicEngine(num_slots)
        self.ir_sensors = {i: IRSensor(i, 10 + i) for i in range(num_slots)}
        self.rfid_reader = RFIDReader()
        self.lcd_display = LCDDisplay()
        self.running = False
        self.update_interval = 2.0  # seconds

    def start(self):
        """Start the parking system"""
        self.running = True
        logger.info("Smart Parking System started")

    def stop(self):
        """Stop the parking system"""
        self.running = False
        logger.info("Smart Parking System stopped")

    def update_display(self):
        """Update LCD display with current status"""
        available = self.logic_engine.get_available_slots()
        total = self.logic_engine.num_slots
        self.lcd_display.display_status(available, total)

    def process_vehicle_entry(self, slot_id: int, rfid: str) -> Tuple[bool, str]:
        """Process vehicle entry"""
        success, message = self.logic_engine.vehicle_entry(slot_id, rfid)
        self.update_display()
        return success, message

    def process_vehicle_exit(self, slot_id: int, rfid: str) -> Tuple[bool, str]:
        """Process vehicle exit"""
        success, message = self.logic_engine.vehicle_exit(slot_id, rfid)
        self.update_display()
        return success, message

    def trigger_ir_sensor(self, slot_id: int):
        """Trigger IR sensor simulation"""
        self.ir_sensors[slot_id].trigger()
        self.logic_engine.ir_trigger(slot_id)

    def get_system_status(self) -> Dict:
        """Get complete system status"""
        return {
            'system_status': 'running' if self.running else 'stopped',
            'timestamp': datetime.now().isoformat(),
            'slots': [s.to_dict() for s in self.logic_engine.get_all_slots_status()],
            'statistics': self.logic_engine.get_statistics(),
            'lcd_display': self.lcd_display.display_buffer
        }

    def export_logs(self, filename: str = 'parking_logs.json'):
        """Export parking logs to JSON"""
        with self.logic_engine.lock:
            logs = {
                'export_timestamp': datetime.now().isoformat(),
                'statistics': self.logic_engine.get_statistics(),
                'entry_exit_logs': self.logic_engine.entry_exit_log,
                'vehicle_history': {
                    rfid: history for rfid, history in self.logic_engine.vehicle_history.items()
                }
            }
        
        with open(filename, 'w') as f:
            json.dump(logs, f, indent=2, default=str)
        logger.info(f"Logs exported to {filename}")


# ============================================================================
# COMPREHENSIVE TEST SUITE
# ============================================================================

class TestSmartParkingSystem:
    """Complete test suite with corner case coverage"""
    
    def __init__(self):
        self.passed = 0
        self.failed = 0
        self.tests_run = 0

    def assert_true(self, condition: bool, message: str):
        """Assert condition is true"""
        self.tests_run += 1
        if condition:
            self.passed += 1
            print(f"✓ PASS: {message}")
        else:
            self.failed += 1
            print(f"✗ FAIL: {message}")

    def assert_equal(self, actual, expected, message: str):
        """Assert equality"""
        self.tests_run += 1
        if actual == expected:
            self.passed += 1
            print(f"✓ PASS: {message}")
        else:
            self.failed += 1
            print(f"✗ FAIL: {message} (expected {expected}, got {actual})")

    def run_all_tests(self):
        """Run complete test suite"""
        print("\n" + "="*70)
        print("SMART PARKING SYSTEM - COMPREHENSIVE TEST SUITE")
        print("="*70 + "\n")
        
        self.test_basic_entry_exit()
        self.test_ir_sensor_debouncing()
        self.test_duplicate_rfid_detection()
        self.test_tamper_detection()
        self.test_slot_occupancy()
        self.test_rfid_validation()
        self.test_concurrent_operations()
        self.test_statistics_tracking()
        self.test_edge_cases()
        
        print("\n" + "="*70)
        print(f"RESULTS: {self.passed} passed, {self.failed} failed out of {self.tests_run} tests")
        print("="*70 + "\n")

    def test_basic_entry_exit(self):
        print("\n--- Test 1: Basic Entry/Exit ---")
        system = SmartParkingSystem(num_slots=10)
        system.start()
        
        # Normal entry
        success, msg = system.process_vehicle_entry(0, "RFID123456")
        self.assert_true(success, "Vehicle entry succeeds")
        
        # Check slot is occupied
        slot = system.logic_engine.get_slot_status(0)
        self.assert_true(slot.is_occupied, "Slot marked as occupied")
        self.assert_equal(slot.vehicle_rfid, "RFID123456", "RFID stored correctly")
        
        # Normal exit
        success, msg = system.process_vehicle_exit(0, "RFID123456")
        self.assert_true(success, "Vehicle exit succeeds")
        self.assert_true(not slot.is_occupied, "Slot marked as empty after exit")

    def test_ir_sensor_debouncing(self):
        print("\n--- Test 2: IR Sensor Debouncing ---")
        sensor = IRSensor(0, 10)
        
        sensor.trigger()
        first_read = sensor.read()
        self.assert_true(first_read, "First IR read succeeds")
        
        # Immediate second read should fail due to debouncing
        second_read = sensor.read()
        self.assert_true(not second_read, "Immediate IR read blocked (debounce)")
        
        # Wait for debounce period
        time.sleep(0.6)
        sensor.trigger()
        delayed_read = sensor.read()
        self.assert_true(delayed_read, "IR read succeeds after debounce period")

    def test_duplicate_rfid_detection(self):
        print("\n--- Test 3: Duplicate RFID Detection ---")
        system = SmartParkingSystem(num_slots=10)
        system.start()
        
        # First entry
        success, msg = system.process_vehicle_entry(0, "RFID789012")
        self.assert_true(success, "First vehicle entry succeeds")
        
        # Immediate second entry attempt (same RFID)
        success, msg = system.process_vehicle_entry(1, "RFID789012")
        self.assert_true(not success, "Duplicate RFID entry rejected")
        self.assert_true("Duplicate" in msg, "Duplicate detection message shown")
        
        # Wait for duplicate window
        time.sleep(2.2)
        success, msg = system.process_vehicle_entry(1, "RFID789012")
        self.assert_true(success, "Entry succeeds after duplicate window expires")

    def test_tamper_detection(self):
        print("\n--- Test 4: Tamper Detection ---")
        system = SmartParkingSystem(num_slots=10)
        system.start()
        
        # Simulate rapid IR triggers without RFID
        for i in range(5):
            system.trigger_ir_sensor(0)
            time.sleep(0.1)
        
        slot = system.logic_engine.get_slot_status(0)
        self.assert_true(slot.is_tampered, "Tamper detected on excessive IR triggers")
        self.assert_true(slot.tamper_count > 0, "Tamper count incremented")
        
        # Subsequent entry should be rejected
        success, msg = system.process_vehicle_entry(0, "RFID111111")
        self.assert_true(not success, "Entry rejected at tampered slot")

    def test_slot_occupancy(self):
        print("\n--- Test 5: Slot Occupancy Management ---")
        system = SmartParkingSystem(num_slots=5)
        system.start()
        
        self.assert_equal(system.logic_engine.get_available_slots(), 5, 
                         "All slots available initially")
        
        # Fill all slots
        for i in range(5):
            system.process_vehicle_entry(i, f"RFID{i:06d}")
        
        self.assert_equal(system.logic_engine.get_available_slots(), 0, 
                         "No slots available when full")
        
        # Try to park in full lot
        success, msg = system.process_vehicle_entry(0, "RFID999999")
        self.assert_true(not success, "Entry rejected when lot is full")
        
        # Free a slot
        system.process_vehicle_exit(0, "RFID000000")
        self.assert_equal(system.logic_engine.get_available_slots(), 1, 
                         "Available slots correct after exit")

    def test_rfid_validation(self):
        print("\n--- Test 6: RFID Validation ---")
        system = SmartParkingSystem(num_slots=10)
        system.start()
        
        # Invalid RFID (too short)
        success, msg = system.process_vehicle_entry(0, "SHORT")
        self.assert_true(not success, "Entry rejected for short RFID")
        
        # Empty RFID
        success, msg = system.process_vehicle_entry(0, "")
        self.assert_true(not success, "Entry rejected for empty RFID")
        
        # Valid RFID
        success, msg = system.process_vehicle_entry(0, "RFID12345678")
        self.assert_true(success, "Entry succeeds with valid RFID")

    def test_concurrent_operations(self):
        print("\n--- Test 7: Concurrent Operations ---")
        system = SmartParkingSystem(num_slots=10)
        system.start()
        
        results = []
        
        def entry_thread(slot_id):
            success, msg = system.process_vehicle_entry(slot_id, f"RFID{slot_id:06d}")
            results.append((slot_id, success))
        
        threads = [threading.Thread(target=entry_thread, args=(i,)) for i in range(5)]
        for t in threads:
            t.start()
        for t in threads:
            t.join()
        
        successful_entries = sum(1 for _, success in results if success)
        self.assert_equal(successful_entries, 5, "All concurrent entries succeed")
        
        available = system.logic_engine.get_available_slots()
        self.assert_equal(available, 5, "Slot count correct after concurrent operations")

    def test_statistics_tracking(self):
        print("\n--- Test 8: Statistics Tracking ---")
        system = SmartParkingSystem(num_slots=10)
        system.start()
        
        # Entry and exit
        system.process_vehicle_entry(0, "RFID111111")
        time.sleep(1)
        system.process_vehicle_exit(0, "RFID111111")
        
        stats = system.logic_engine.get_statistics()
        self.assert_equal(stats['total_entries'], 1, "Entry count tracked")
        self.assert_equal(stats['total_exits'], 1, "Exit count tracked")
        self.assert_equal(stats['unique_vehicles'], 1, "Unique vehicle count tracked")
        self.assert_true(stats['average_duration_minutes'] > 0, 
                        "Parking duration calculated")

    def test_edge_cases(self):
        print("\n--- Test 9: Edge Cases ---")
        system = SmartParkingSystem(num_slots=10)
        system.start()
        
        # Exit from empty slot
        success, msg = system.process_vehicle_exit(0, "RFID000000")
        self.assert_true(not success, "Exit rejected from empty slot")
        
        # Invalid slot ID
        success, msg = system.process_vehicle_entry(-1, "RFID123456")
        self.assert_true(not success, "Invalid slot ID rejected")
        
        success, msg = system.process_vehicle_entry(100, "RFID123456")
        self.assert_true(not success, "Out of range slot ID rejected")
        
        # RFID mismatch on exit
        system.process_vehicle_entry(0, "RFID111111")
        success, msg = system.process_vehicle_exit(0, "RFID222222")
        self.assert_true(not success, "Exit rejected for RFID mismatch")


if __name__ == "__main__":
    # Run test suite
    tester = TestSmartParkingSystem()
    tester.run_all_tests()
    
    # Demo system
    print("\n" + "="*70)
    print("SYSTEM DEMONSTRATION")
    print("="*70 + "\n")
    
    system = SmartParkingSystem(num_slots=8)
    system.start()
    
    # Simulate parking operations
    print("\n1. Vehicle enters Slot 0:")
    system.process_vehicle_entry(0, "RFID1001")
    system.update_display()
    
    print("\n2. Vehicle enters Slot 1:")
    system.process_vehicle_entry(1, "RFID1002")
    system.update_display()
    
    print("\n3. System Status:")
    status = system
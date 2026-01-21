#!/usr/bin/env python3
"""
Sysfs Reader Module
Reads device health metrics from kernel sysfs interface
"""

from pathlib import Path
from typing import Dict, Optional


class SysfsReader:
    """Reads health metrics from sysfs"""
    
    SYSFS_BASE = Path('/sys/kernel/device_health')
    
    def __init__(self):
        """Initialize sysfs reader"""
        if not self.SYSFS_BASE.exists():
            raise RuntimeError("Device health sysfs interface not found")
    
    def read_value(self, attribute: str) -> Optional[str]:
        """Read a single sysfs attribute"""
        try:
            path = self.SYSFS_BASE / attribute
            with open(path, 'r') as f:
                return f.read().strip()
        except FileNotFoundError:
            return None
        except Exception as e:
            print(f"Error reading {attribute}: {e}")
            return None
    
    def read_int(self, attribute: str) -> Optional[int]:
        """Read an integer value from sysfs"""
        value = self.read_value(attribute)
        if value is not None:
            try:
                return int(value)
            except ValueError:
                return None
        return None
    
    def read_float(self, attribute: str) -> Optional[float]:
        """Read a float value from sysfs"""
        value = self.read_value(attribute)
        if value is not None:
            try:
                return float(value)
            except ValueError:
                return None
        return None
    
    def read_all_metrics(self) -> Optional[Dict]:
        """Read all available health metrics"""
        try:
            metrics = {
                # Overall health
                'overall_health': self.read_int('overall_health'),
                'health_status': self.read_value('health_status'),
                
                # Thermal metrics
                'cpu_temp': self.read_int('cpu_temp'),
                'gpu_temp': self.read_int('gpu_temp'),
                
                # Memory metrics
                'mem_usage': self.read_int('mem_usage'),
                
                # CPU metrics
                'cpu_load': self.read_int('cpu_load'),
                
                # GPU metrics
                'gpu_usage': self.read_int('gpu_usage'),
                
                # Storage metrics
                'storage_usage': self.read_int('storage_usage'),
                
                # Alert flags
                'alert_flags': self.read_value('alert_flags'),
            }
            
            # Convert temperatures from millidegrees to degrees
            if metrics['cpu_temp'] is not None:
                metrics['cpu_temp'] = metrics['cpu_temp'] / 1000.0
            
            if metrics['gpu_temp'] is not None:
                metrics['gpu_temp'] = metrics['gpu_temp'] / 1000.0
            
            return metrics
            
        except Exception as e:
            print(f"Error reading metrics: {e}")
            return None
    
    def read_full_report(self) -> Optional[str]:
        """Read the full health report"""
        return self.read_value('full_report')
    
    def get_alert_flags(self) -> int:
        """Get current alert flags as integer"""
        flags_str = self.read_value('alert_flags')
        if flags_str:
            try:
                return int(flags_str, 16)  # Parse hex string
            except ValueError:
                return 0
        return 0
    
    def is_alert_active(self, alert_flag: int) -> bool:
        """Check if specific alert flag is set"""
        current_flags = self.get_alert_flags()
        return (current_flags & alert_flag) != 0
    
    def get_update_interval(self) -> Optional[int]:
        """Get current update interval in seconds"""
        return self.read_int('update_interval')
    
    def set_update_interval(self, seconds: int) -> bool:
        """Set update interval (requires root privileges)"""
        try:
            path = self.SYSFS_BASE / 'update_interval'
            with open(path, 'w') as f:
                f.write(str(seconds))
            return True
        except Exception as e:
            print(f"Error setting update interval: {e}")
            return False


# Alert flag constants (must match kernel module)
ALERT_THERMAL_CRITICAL = 1 << 0
ALERT_THERMAL_WARNING = 1 << 1
ALERT_MEMORY_CRITICAL = 1 << 2
ALERT_MEMORY_WARNING = 1 << 3
ALERT_CPU_CRITICAL = 1 << 4
ALERT_CPU_WARNING = 1 << 5
ALERT_GPU_CRITICAL = 1 << 6
ALERT_GPU_WARNING = 1 << 7
ALERT_STORAGE_CRITICAL = 1 << 8
ALERT_STORAGE_WARNING = 1 << 9


def main():
    """Test sysfs reader"""
    try:
        reader = SysfsReader()
        
        print("=== Device Health Metrics ===")
        metrics = reader.read_all_metrics()
        
        if metrics:
            for key, value in metrics.items():
                print(f"{key}: {value}")
        else:
            print("Failed to read metrics")
        
        print("\n=== Full Report ===")
        report = reader.read_full_report()
        if report:
            print(report)
        
    except Exception as e:
        print(f"Error: {e}")


if __name__ == '__main__':
    main()
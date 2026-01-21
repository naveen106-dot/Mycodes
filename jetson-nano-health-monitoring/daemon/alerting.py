#!/usr/bin/env python3
"""
Alert Manager Module
Generates and manages alerts based on health metrics
"""

import time
from typing import Dict, List
from datetime import datetime, timedelta


class AlertManager:
    """Manages health monitoring alerts"""
    
    def __init__(self, config: Dict):
        """Initialize alert manager"""
        self.config = config
        self.critical_threshold = config.get('critical_threshold', 30)
        self.warning_threshold = config.get('warning_threshold', 60)
        self.cooldown = config.get('cooldown', 300)  # 5 minutes default
        
        # Track alert history to implement cooldown
        self.alert_history = {}
        
        # Alert thresholds for different metrics
        self.thresholds = {
            'thermal': {
                'critical': 85,  # Celsius
                'warning': 75
            },
            'memory': {
                'critical': 95,  # Percent
                'warning': 85
            },
            'cpu': {
                'critical': 95,  # Percent
                'warning': 80
            },
            'gpu': {
                'critical': 95,  # Percent
                'warning': 80
            },
            'storage': {
                'critical': 95,  # Percent
                'warning': 85
            }
        }
    
    def can_send_alert(self, alert_key: str) -> bool:
        """Check if alert can be sent (cooldown logic)"""
        if alert_key not in self.alert_history:
            return True
        
        last_sent = self.alert_history[alert_key]
        elapsed = time.time() - last_sent
        
        return elapsed >= self.cooldown
    
    def record_alert(self, alert_key: str):
        """Record that an alert was sent"""
        self.alert_history[alert_key] = time.time()
    
    def check_thermal_alerts(self, metrics: Dict) -> List[Dict]:
        """Check for thermal-related alerts"""
        alerts = []
        
        cpu_temp = metrics.get('cpu_temp', 0)
        gpu_temp = metrics.get('gpu_temp', 0)
        
        # CPU temperature alerts
        if cpu_temp >= self.thresholds['thermal']['critical']:
            alert_key = 'thermal_cpu_critical'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'THERMAL',
                    'severity': 'CRITICAL',
                    'component': 'CPU',
                    'message': f'Critical CPU temperature: {cpu_temp:.1f}°C',
                    'value': cpu_temp,
                    'threshold': self.thresholds['thermal']['critical']
                })
                self.record_alert(alert_key)
        
        elif cpu_temp >= self.thresholds['thermal']['warning']:
            alert_key = 'thermal_cpu_warning'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'THERMAL',
                    'severity': 'WARNING',
                    'component': 'CPU',
                    'message': f'High CPU temperature: {cpu_temp:.1f}°C',
                    'value': cpu_temp,
                    'threshold': self.thresholds['thermal']['warning']
                })
                self.record_alert(alert_key)
        
        # GPU temperature alerts
        if gpu_temp >= self.thresholds['thermal']['critical']:
            alert_key = 'thermal_gpu_critical'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'THERMAL',
                    'severity': 'CRITICAL',
                    'component': 'GPU',
                    'message': f'Critical GPU temperature: {gpu_temp:.1f}°C',
                    'value': gpu_temp,
                    'threshold': self.thresholds['thermal']['critical']
                })
                self.record_alert(alert_key)
        
        elif gpu_temp >= self.thresholds['thermal']['warning']:
            alert_key = 'thermal_gpu_warning'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'THERMAL',
                    'severity': 'WARNING',
                    'component': 'GPU',
                    'message': f'High GPU temperature: {gpu_temp:.1f}°C',
                    'value': gpu_temp,
                    'threshold': self.thresholds['thermal']['warning']
                })
                self.record_alert(alert_key)
        
        return alerts
    
    def check_memory_alerts(self, metrics: Dict) -> List[Dict]:
        """Check for memory-related alerts"""
        alerts = []
        mem_usage = metrics.get('mem_usage', 0)
        
        if mem_usage >= self.thresholds['memory']['critical']:
            alert_key = 'memory_critical'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'MEMORY',
                    'severity': 'CRITICAL',
                    'component': 'MEMORY',
                    'message': f'Critical memory usage: {mem_usage}%',
                    'value': mem_usage,
                    'threshold': self.thresholds['memory']['critical']
                })
                self.record_alert(alert_key)
        
        elif mem_usage >= self.thresholds['memory']['warning']:
            alert_key = 'memory_warning'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'MEMORY',
                    'severity': 'WARNING',
                    'component': 'MEMORY',
                    'message': f'High memory usage: {mem_usage}%',
                    'value': mem_usage,
                    'threshold': self.thresholds['memory']['warning']
                })
                self.record_alert(alert_key)
        
        return alerts
    
    def check_cpu_alerts(self, metrics: Dict) -> List[Dict]:
        """Check for CPU-related alerts"""
        alerts = []
        cpu_load = metrics.get('cpu_load', 0)
        
        if cpu_load >= self.thresholds['cpu']['critical']:
            alert_key = 'cpu_critical'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'CPU',
                    'severity': 'CRITICAL',
                    'component': 'CPU',
                    'message': f'Critical CPU load: {cpu_load}%',
                    'value': cpu_load,
                    'threshold': self.thresholds['cpu']['critical']
                })
                self.record_alert(alert_key)
        
        elif cpu_load >= self.thresholds['cpu']['warning']:
            alert_key = 'cpu_warning'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'CPU',
                    'severity': 'WARNING',
                    'component': 'CPU',
                    'message': f'High CPU load: {cpu_load}%',
                    'value': cpu_load,
                    'threshold': self.thresholds['cpu']['warning']
                })
                self.record_alert(alert_key)
        
        return alerts
    
    def check_gpu_alerts(self, metrics: Dict) -> List[Dict]:
        """Check for GPU-related alerts"""
        alerts = []
        gpu_usage = metrics.get('gpu_usage', 0)
        
        if gpu_usage >= self.thresholds['gpu']['critical']:
            alert_key = 'gpu_critical'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'GPU',
                    'severity': 'CRITICAL',
                    'component': 'GPU',
                    'message': f'Critical GPU usage: {gpu_usage}%',
                    'value': gpu_usage,
                    'threshold': self.thresholds['gpu']['critical']
                })
                self.record_alert(alert_key)
        
        elif gpu_usage >= self.thresholds['gpu']['warning']:
            alert_key = 'gpu_warning'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'GPU',
                    'severity': 'WARNING',
                    'component': 'GPU',
                    'message': f'High GPU usage: {gpu_usage}%',
                    'value': gpu_usage,
                    'threshold': self.thresholds['gpu']['warning']
                })
                self.record_alert(alert_key)
        
        return alerts
    
    def check_storage_alerts(self, metrics: Dict) -> List[Dict]:
        """Check for storage-related alerts"""
        alerts = []
        storage_usage = metrics.get('storage_usage', 0)
        
        if storage_usage >= self.thresholds['storage']['critical']:
            alert_key = 'storage_critical'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'STORAGE',
                    'severity': 'CRITICAL',
                    'component': 'STORAGE',
                    'message': f'Critical storage usage: {storage_usage}%',
                    'value': storage_usage,
                    'threshold': self.thresholds['storage']['critical']
                })
                self.record_alert(alert_key)
        
        elif storage_usage >= self.thresholds['storage']['warning']:
            alert_key = 'storage_warning'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'STORAGE',
                    'severity': 'WARNING',
                    'component': 'STORAGE',
                    'message': f'High storage usage: {storage_usage}%',
                    'value': storage_usage,
                    'threshold': self.thresholds['storage']['warning']
                })
                self.record_alert(alert_key)
        
        return alerts
    
    def check_overall_health_alerts(self, metrics: Dict) -> List[Dict]:
        """Check overall health score"""
        alerts = []
        health = metrics.get('overall_health', 100)
        
        if health <= self.critical_threshold:
            alert_key = 'health_critical'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'HEALTH',
                    'severity': 'CRITICAL',
                    'component': 'SYSTEM',
                    'message': f'Critical system health: {health}',
                    'value': health,
                    'threshold': self.critical_threshold
                })
                self.record_alert(alert_key)
        
        elif health <= self.warning_threshold:
            alert_key = 'health_warning'
            if self.can_send_alert(alert_key):
                alerts.append({
                    'type': 'HEALTH',
                    'severity': 'WARNING',
                    'component': 'SYSTEM',
                    'message': f'System health declining: {health}',
                    'value': health,
                    'threshold': self.warning_threshold
                })
                self.record_alert(alert_key)
        
        return alerts
    
    def check_alerts(self, metrics: Dict) -> List[Dict]:
        """Check all alert conditions"""
        alerts = []
        
        alerts.extend(self.check_thermal_alerts(metrics))
        alerts.extend(self.check_memory_alerts(metrics))
        alerts.extend(self.check_cpu_alerts(metrics))
        alerts.extend(self.check_gpu_alerts(metrics))
        alerts.extend(self.check_storage_alerts(metrics))
        alerts.extend(self.check_overall_health_alerts(metrics))
        
        return alerts


def main():
    """Test alert manager"""
    config = {
        'critical_threshold': 30,
        'warning_threshold': 60,
        'cooldown': 10  # Short cooldown for testing
    }
    
    manager = AlertManager(config)
    
    # Test with critical metrics
    test_metrics = {
        'cpu_temp': 90.0,
        'gpu_temp': 88.0,
        'mem_usage': 96,
        'cpu_load': 85,
        'gpu_usage': 92,
        'storage_usage': 94,
        'overall_health': 25
    }
    
    print("Checking alerts for critical metrics...")
    alerts = manager.check_alerts(test_metrics)
    
    for alert in alerts:
        print(f"[{alert['severity']}] {alert['type']}: {alert['message']}")


if __name__ == '__main__':
    main()
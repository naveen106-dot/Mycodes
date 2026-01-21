#!/usr/bin/env python3
"""
Health Logger Module
Logging implementation for device health monitoring
"""

import logging
import logging.handlers
from pathlib import Path
from datetime import datetime
from typing import Dict


class HealthLogger:
    """Logger for device health monitoring"""
    
    def __init__(self, config: Dict):
        """Initialize logger with configuration"""
        self.config = config
        self.log_dir = Path(config.get('log_dir', '/var/log/device-health'))
        self.log_dir.mkdir(parents=True, exist_ok=True)
        
        # Setup loggers
        self.main_logger = self._setup_logger(
            'daemon',
            self.log_dir / 'daemon.log'
        )
        self.alert_logger = self._setup_logger(
            'alerts',
            self.log_dir / 'alerts.log'
        )
        self.metrics_logger = self._setup_logger(
            'metrics',
            self.log_dir / 'metrics.log'
        )
        self.error_logger = self._setup_logger(
            'errors',
            self.log_dir / 'errors.log',
            level=logging.ERROR
        )
    
    def _setup_logger(self, name: str, log_file: Path, 
                     level=None) -> logging.Logger:
        """Setup individual logger with rotation"""
        logger = logging.getLogger(name)
        
        if level is None:
            level_str = self.config.get('level', 'INFO')
            level = getattr(logging, level_str.upper())
        
        logger.setLevel(level)
        
        # Rotating file handler
        max_bytes = self.config.get('max_size', 10485760)  # 10MB
        backup_count = self.config.get('backup_count', 5)
        
        handler = logging.handlers.RotatingFileHandler(
            log_file,
            maxBytes=max_bytes,
            backupCount=backup_count
        )
        
        # Formatter
        formatter = logging.Formatter(
            '%(asctime)s - %(name)s - %(levelname)s - %(message)s',
            datefmt='%Y-%m-%d %H:%M:%S'
        )
        handler.setFormatter(formatter)
        
        logger.addHandler(handler)
        
        # Also log to console in debug mode
        if self.config.get('console', False):
            console_handler = logging.StreamHandler()
            console_handler.setFormatter(formatter)
            logger.addHandler(console_handler)
        
        return logger
    
    def info(self, message: str):
        """Log info message"""
        self.main_logger.info(message)
    
    def warning(self, message: str):
        """Log warning message"""
        self.main_logger.warning(message)
        self.alert_logger.warning(message)
    
    def error(self, message: str):
        """Log error message"""
        self.main_logger.error(message)
        self.error_logger.error(message)
    
    def critical(self, message: str):
        """Log critical message"""
        self.main_logger.critical(message)
        self.error_logger.critical(message)
        self.alert_logger.critical(message)
    
    def debug(self, message: str):
        """Log debug message"""
        self.main_logger.debug(message)
    
    def log_metrics(self, metrics: Dict):
        """Log metrics data"""
        timestamp = datetime.now().isoformat()
        
        # Format metrics in a structured way
        metrics_str = (
            f"health={metrics.get('overall_health', 'N/A')} "
            f"cpu_temp={metrics.get('cpu_temp', 'N/A')}C "
            f"gpu_temp={metrics.get('gpu_temp', 'N/A')}C "
            f"mem={metrics.get('mem_usage', 'N/A')}% "
            f"cpu_load={metrics.get('cpu_load', 'N/A')}% "
            f"gpu={metrics.get('gpu_usage', 'N/A')}% "
            f"storage={metrics.get('storage_usage', 'N/A')}%"
        )
        
        self.metrics_logger.info(f"{timestamp} | {metrics_str}")
    
    def log_alert(self, alert_type: str, message: str, severity: str):
        """Log alert with details"""
        timestamp = datetime.now().isoformat()
        alert_str = f"[{severity}] {alert_type}: {message}"
        
        if severity.upper() == 'CRITICAL':
            self.critical(alert_str)
        elif severity.upper() == 'WARNING':
            self.warning(alert_str)
        else:
            self.info(alert_str)
    
    def log_anomaly(self, metric: str, expected: float, 
                   actual: float, deviation: float):
        """Log detected anomaly"""
        message = (
            f"Anomaly detected in {metric}: "
            f"expected={expected:.2f}, actual={actual:.2f}, "
            f"deviation={deviation:.2f}σ"
        )
        self.warning(message)
    
    def close(self):
        """Close all log handlers"""
        for logger in [self.main_logger, self.alert_logger, 
                      self.metrics_logger, self.error_logger]:
            for handler in logger.handlers:
                handler.close()
                logger.removeHandler(handler)


def main():
    """Test logger"""
    config = {
        'level': 'DEBUG',
        'log_dir': '/tmp/device-health-test',
        'max_size': 1048576,  # 1MB
        'backup_count': 3,
        'console': True
    }
    
    logger = HealthLogger(config)
    
    logger.info("Testing info message")
    logger.warning("Testing warning message")
    logger.error("Testing error message")
    
    test_metrics = {
        'overall_health': 85,
        'cpu_temp': 55.5,
        'gpu_temp': 52.3,
        'mem_usage': 65,
        'cpu_load': 45,
        'gpu_usage': 30,
        'storage_usage': 70
    }
    
    logger.log_metrics(test_metrics)
    logger.log_alert('THERMAL', 'High CPU temperature', 'WARNING')
    logger.log_anomaly('cpu_load', 50.0, 85.0, 2.5)
    
    logger.close()
    print(f"Logs written to {config['log_dir']}")


if __name__ == '__main__':
    main()
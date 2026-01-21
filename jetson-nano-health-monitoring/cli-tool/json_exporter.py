#!/usr/bin/env python3
"""
JSON Exporter Module
Exports health metrics in JSON format
"""

import json
from datetime import datetime
from pathlib import Path
from typing import Dict, Optional


class JSONExporter:
    """Exports health metrics to JSON format"""
    
    def __init__(self):
        """Initialize JSON exporter"""
        pass
    
    def export(self, metrics: Dict, output_path: Optional[str] = None) -> str:
        """
        Export metrics to JSON
        
        Args:
            metrics: Health metrics dictionary
            output_path: Optional file path to save JSON
        
        Returns:
            JSON string
        """
        # Enhance metrics with metadata
        export_data = {
            'metadata': {
                'version': '1.0.0',
                'timestamp': datetime.now().isoformat(),
                'device': 'Jetson Nano',
                'export_type': 'health_metrics'
            },
            'metrics': metrics
        }
        
        # Convert to JSON
        json_str = json.dumps(export_data, indent=2, sort_keys=True)
        
        # Save to file if path provided
        if output_path:
            output_file = Path(output_path)
            output_file.parent.mkdir(parents=True, exist_ok=True)
            
            with open(output_file, 'w') as f:
                f.write(json_str)
        
        return json_str
    
    def export_compact(self, metrics: Dict, output_path: Optional[str] = None) -> str:
        """
        Export metrics in compact JSON format
        
        Args:
            metrics: Health metrics dictionary
            output_path: Optional file path to save JSON
        
        Returns:
            Compact JSON string
        """
        # Compact format without metadata
        json_str = json.dumps(metrics, separators=(',', ':'))
        
        if output_path:
            with open(output_path, 'w') as f:
                f.write(json_str)
        
        return json_str
    
    def export_time_series(self, metrics_list: list, 
                          output_path: Optional[str] = None) -> str:
        """
        Export multiple metrics as time series
        
        Args:
            metrics_list: List of metrics dictionaries with timestamps
            output_path: Optional file path to save JSON
        
        Returns:
            JSON string
        """
        export_data = {
            'metadata': {
                'version': '1.0.0',
                'export_timestamp': datetime.now().isoformat(),
                'device': 'Jetson Nano',
                'export_type': 'time_series',
                'sample_count': len(metrics_list)
            },
            'time_series': metrics_list
        }
        
        json_str = json.dumps(export_data, indent=2)
        
        if output_path:
            with open(output_path, 'w') as f:
                f.write(json_str)
        
        return json_str
    
    def load(self, input_path: str) -> Optional[Dict]:
        """
        Load metrics from JSON file
        
        Args:
            input_path: Path to JSON file
        
        Returns:
            Metrics dictionary or None if error
        """
        try:
            with open(input_path, 'r') as f:
                data = json.load(f)
            
            # Extract metrics from full export or return raw data
            if 'metrics' in data:
                return data['metrics']
            else:
                return data
        
        except Exception as e:
            print(f"Error loading JSON: {e}")
            return None


def main():
    """Test JSON exporter"""
    exporter = JSONExporter()
    
    # Test data
    test_metrics = {
        'overall_health': 85,
        'health_status': 'EXCELLENT',
        'cpu_temp': 55.2,
        'gpu_temp': 52.8,
        'mem_usage': 65,
        'cpu_load': 45,
        'gpu_usage': 30,
        'storage_usage': 70,
        'alert_flags': '0x00000000'
    }
    
    # Test standard export
    print("Standard Export:")
    print(exporter.export(test_metrics))
    print()
    
    # Test compact export
    print("Compact Export:")
    print(exporter.export_compact(test_metrics))
    print()
    
    # Test time series export
    time_series = []
    for i in range(3):
        entry = test_metrics.copy()
        entry['timestamp'] = datetime.now().isoformat()
        entry['cpu_temp'] += i
        time_series.append(entry)
    
    print("Time Series Export:")
    print(exporter.export_time_series(time_series))
    
    # Test file export
    test_file = '/tmp/health_metrics_test.json'
    exporter.export(test_metrics, test_file)
    print(f"\nExported to: {test_file}")
    
    # Test loading
    loaded = exporter.load(test_file)
    if loaded:
        print("\nLoaded metrics:")
        print(json.dumps(loaded, indent=2))


if __name__ == '__main__':
    main()
    
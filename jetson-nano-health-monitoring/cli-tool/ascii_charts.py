#!/usr/bin/env python3
"""
ASCII Charts Module
Generates terminal-based charts for health metrics visualization
"""

import csv
from datetime import datetime
from typing import List, Tuple


class ASCIICharts:
    """Generate ASCII charts for terminal display"""
    
    def __init__(self, width: int = 60, height: int = 15):
        """
        Initialize ASCII chart generator
        
        Args:
            width: Chart width in characters
            height: Chart height in lines
        """
        self.width = width
        self.height = height
        self.data = {}
    
    def load_csv(self, csv_path: str):
        """Load data from CSV file"""
        self.data = {
            'timestamp': [],
            'overall_health': [],
            'cpu_temp': [],
            'gpu_temp': [],
            'mem_usage': [],
            'cpu_load': [],
            'gpu_usage': [],
            'storage_usage': []
        }
        
        try:
            with open(csv_path, 'r') as f:
                reader = csv.DictReader(f)
                for row in reader:
                    self.data['timestamp'].append(row['timestamp'])
                    self.data['overall_health'].append(float(row['overall_health']))
                    self.data['cpu_temp'].append(float(row['cpu_temp']))
                    self.data['gpu_temp'].append(float(row['gpu_temp']))
                    self.data['mem_usage'].append(float(row['mem_usage']))
                    self.data['cpu_load'].append(float(row['cpu_load']))
                    self.data['gpu_usage'].append(float(row['gpu_usage']))
                    self.data['storage_usage'].append(float(row['storage_usage']))
        except Exception as e:
            print(f"Error loading CSV: {e}")
    
    def _normalize_data(self, data: List[float], 
                       min_val: float = None, 
                       max_val: float = None) -> List[int]:
        """Normalize data to chart height"""
        if not data:
            return []
        
        if min_val is None:
            min_val = min(data)
        if max_val is None:
            max_val = max(data)
        
        if max_val == min_val:
            return [self.height // 2] * len(data)
        
        normalized = []
        for val in data:
            norm = int(((val - min_val) / (max_val - min_val)) * (self.height - 1))
            normalized.append(norm)
        
        return normalized
    
    def _create_line_chart(self, data: List[float], 
                          title: str,
                          y_label: str = "",
                          min_val: float = None,
                          max_val: float = None) -> str:
        """Create a line chart"""
        if not data:
            return "No data available"
        
        # Limit data points to chart width
        if len(data) > self.width:
            step = len(data) // self.width
            data = data[::step][:self.width]
        
        # Normalize data
        normalized = self._normalize_data(data, min_val, max_val)
        
        # Determine actual min/max for labeling
        actual_min = min_val if min_val is not None else min(data)
        actual_max = max_val if max_val is not None else max(data)
        
        # Build chart
        lines = []
        
        # Title
        lines.append(f"┌─ {title} " + "─" * (self.width - len(title) - 4) + "┐")
        
        # Y-axis and plot
        for y in range(self.height - 1, -1, -1):
            # Y-axis label
            if y == self.height - 1:
                label = f"{actual_max:6.1f}"
            elif y == 0:
                label = f"{actual_min:6.1f}"
            elif y == self.height // 2:
                label = f"{(actual_max + actual_min) / 2:6.1f}"
            else:
                label = " " * 6
            
            # Plot line
            line = label + " │"
            
            for x, norm_val in enumerate(normalized):
                if norm_val == y:
                    line += "●"
                elif norm_val > y:
                    line += "│"
                else:
                    line += " "
            
            lines.append(line)
        
        # X-axis
        lines.append(" " * 7 + "└" + "─" * self.width + "┘")
        
        # Stats
        current = data[-1]
        avg = sum(data) / len(data)
        lines.append(f"  Current: {current:.1f} | Avg: {avg:.1f} | "
                    f"Min: {actual_min:.1f} | Max: {actual_max:.1f}")
        
        return "\n".join(lines)
    
    def _create_bar_chart(self, value: float, max_value: float, 
                         label: str, unit: str = "%") -> str:
        """Create a horizontal bar chart"""
        bar_width = 40
        filled = int((value / max_value) * bar_width)
        empty = bar_width - filled
        
        # Color based on value
        if value >= 90:
            bar_char = '█'  # Critical
        elif value >= 75:
            bar_char = '▓'  # Warning
        else:
            bar_char = '▒'  # Normal
        
        bar = bar_char * filled + '░' * empty
        
        return f"{label:15s} │{bar}│ {value:.1f}{unit}"
    
    def display_metric(self, metric: str):
        """Display chart for a specific metric"""
        if metric not in self.data or not self.data[metric]:
            print(f"No data available for {metric}")
            return
        
        data = self.data[metric]
        
        # Set appropriate min/max based on metric type
        if 'temp' in metric:
            min_val, max_val = 0, 100
            unit = "°C"
        elif 'health' in metric:
            min_val, max_val = 0, 100
            unit = ""
        else:
            min_val, max_val = 0, 100
            unit = "%"
        
        title = metric.replace('_', ' ').upper()
        chart = self._create_line_chart(data, title, unit, min_val, max_val)
        print(chart)
        print()
    
    def display_all(self):
        """Display charts for all metrics"""
        metrics = ['overall_health', 'cpu_temp', 'gpu_temp', 
                  'mem_usage', 'cpu_load', 'gpu_usage', 'storage_usage']
        
        print("\n" + "=" * 70)
        print("         DEVICE HEALTH METRICS CHARTS")
        print("=" * 70 + "\n")
        
        for metric in metrics:
            if metric in self.data and self.data[metric]:
                self.display_metric(metric)
    
    def display_current_status(self, metrics: dict):
        """Display current status as bar charts"""
        print("\n" + "=" * 70)
        print("         CURRENT STATUS")
        print("=" * 70 + "\n")
        
        print(self._create_bar_chart(
            metrics.get('overall_health', 0), 100, 
            "Health Score", ""
        ))
        print()
        print(self._create_bar_chart(
            metrics.get('cpu_temp', 0), 100, 
            "CPU Temp", "°C"
        ))
        print(self._create_bar_chart(
            metrics.get('gpu_temp', 0), 100, 
            "GPU Temp", "°C"
        ))
        print()
        print(self._create_bar_chart(
            metrics.get('mem_usage', 0), 100, 
            "Memory", "%"
        ))
        print(self._create_bar_chart(
            metrics.get('cpu_load', 0), 100, 
            "CPU Load", "%"
        ))
        print(self._create_bar_chart(
            metrics.get('gpu_usage', 0), 100, 
            "GPU Usage", "%"
        ))
        print(self._create_bar_chart(
            metrics.get('storage_usage', 0), 100, 
            "Storage", "%"
        ))
        print()


def main():
    """Test ASCII charts"""
    import random
    
    charts = ASCIICharts(width=60, height=15)
    
    # Generate test data
    print("Generating test data...")
    test_data = []
    for i in range(100):
        base = 50 + 30 * (i / 100)  # Trend upward
        noise = random.uniform(-10, 10)
        test_data.append(max(0, min(100, base + noise)))
    
    # Create test data structure
    charts.data = {
        'overall_health': test_data,
        'cpu_temp': [x * 0.8 for x in test_data],
        'mem_usage': [x * 0.9 for x in test_data]
    }
    
    # Display charts
    charts.display_metric('overall_health')
    charts.display_metric('cpu_temp')
    
    # Display current status
    current_metrics = {
        'overall_health': 75,
        'cpu_temp': 65.5,
        'gpu_temp': 62.3,
        'mem_usage': 70,
        'cpu_load': 55,
        'gpu_usage': 40,
        'storage_usage': 65
    }
    
    charts.display_current_status(current_metrics)


if __name__ == '__main__':
    main()
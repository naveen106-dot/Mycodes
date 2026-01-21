#!/usr/bin/env python3
"""
Anomaly Detector Module
Detects anomalies in device health metrics using statistical analysis
"""

import statistics
from collections import deque
from typing import Dict, List, Optional


class AnomalyDetector:
    """Detects anomalies in time-series health data"""
    
    def __init__(self, config: Dict):
        """Initialize anomaly detector"""
        self.config = config
        self.window_size = config.get('window_size', 60)
        self.sensitivity = config.get('sensitivity', 2.0)  # Sigma threshold
        
        # Historical data windows for each metric
        self.history = {
            'cpu_temp': deque(maxlen=self.window_size),
            'gpu_temp': deque(maxlen=self.window_size),
            'mem_usage': deque(maxlen=self.window_size),
            'cpu_load': deque(maxlen=self.window_size),
            'gpu_usage': deque(maxlen=self.window_size),
            'storage_usage': deque(maxlen=self.window_size),
            'overall_health': deque(maxlen=self.window_size)
        }
        
        # Track detected anomalies
        self.anomaly_count = {key: 0 for key in self.history.keys()}
    
    def update_history(self, metrics: Dict):
        """Update historical data with new metrics"""
        for key in self.history.keys():
            if key in metrics and metrics[key] is not None:
                self.history[key].append(metrics[key])
    
    def calculate_statistics(self, metric: str) -> Optional[tuple]:
        """Calculate mean and standard deviation for a metric"""
        data = list(self.history[metric])
        
        if len(data) < 10:  # Need minimum data points
            return None
        
        try:
            mean = statistics.mean(data)
            if len(data) < 2:
                return None
            stdev = statistics.stdev(data)
            return (mean, stdev)
        except statistics.StatisticsError:
            return None
    
    def detect_metric_anomaly(self, metric: str, current_value: float) -> Optional[Dict]:
        """Detect if current value is anomalous"""
        stats = self.calculate_statistics(metric)
        
        if stats is None:
            return None
        
        mean, stdev = stats
        
        # If stdev is too small, no anomaly detection
        if stdev < 0.1:
            return None
        
        # Calculate z-score
        z_score = abs((current_value - mean) / stdev)
        
        # Check if exceeds sensitivity threshold
        if z_score > self.sensitivity:
            self.anomaly_count[metric] += 1
            
            return {
                'metric': metric,
                'current_value': current_value,
                'expected_value': mean,
                'std_deviation': stdev,
                'z_score': z_score,
                'deviation': z_score,
                'message': f'{metric} anomaly: {current_value:.2f} '
                          f'(expected: {mean:.2f} ± {stdev:.2f})'
            }
        
        return None
    
    def detect_rapid_change(self, metric: str, current_value: float) -> Optional[Dict]:
        """Detect rapid changes in metrics"""
        data = list(self.history[metric])
        
        if len(data) < 5:
            return None
        
        # Check last few values for rapid change
        recent = data[-5:]
        if len(recent) < 2:
            return None
        
        # Calculate rate of change
        changes = [abs(recent[i] - recent[i-1]) for i in range(1, len(recent))]
        avg_change = statistics.mean(changes)
        
        # Current change
        last_value = recent[-1]
        current_change = abs(current_value - last_value)
        
        # If current change is significantly larger
        if avg_change > 0 and current_change > avg_change * 3:
            return {
                'metric': metric,
                'current_value': current_value,
                'previous_value': last_value,
                'change': current_change,
                'avg_change': avg_change,
                'message': f'Rapid change in {metric}: '
                          f'{last_value:.2f} → {current_value:.2f}'
            }
        
        return None
    
    def detect_sustained_degradation(self, metric: str) -> Optional[Dict]:
        """Detect sustained degradation in health metrics"""
        data = list(self.history[metric])
        
        if len(data) < 20:
            return None
        
        # Check for downward trend (for health scores)
        # or upward trend (for usage/temperature)
        recent = data[-20:]
        
        # Simple trend detection: compare first half vs second half
        first_half = recent[:10]
        second_half = recent[-10:]
        
        first_avg = statistics.mean(first_half)
        second_avg = statistics.mean(second_half)
        
        # For health scores, check if declining
        if 'health' in metric:
            if first_avg - second_avg > 10:  # Health dropped by 10 points
                return {
                    'metric': metric,
                    'trend': 'declining',
                    'from': first_avg,
                    'to': second_avg,
                    'change': first_avg - second_avg,
                    'message': f'Sustained health degradation: '
                              f'{first_avg:.1f} → {second_avg:.1f}'
                }
        
        # For usage/temperature, check if increasing
        elif any(x in metric for x in ['temp', 'usage', 'load']):
            if second_avg - first_avg > 10:  # Usage increased by 10%
                return {
                    'metric': metric,
                    'trend': 'increasing',
                    'from': first_avg,
                    'to': second_avg,
                    'change': second_avg - first_avg,
                    'message': f'Sustained increase in {metric}: '
                              f'{first_avg:.1f} → {second_avg:.1f}'
                }
        
        return None
    
    def detect(self, metrics: Dict) -> List[Dict]:
        """Detect all types of anomalies"""
        anomalies = []
        
        # Update historical data
        self.update_history(metrics)
        
        # Check each metric for anomalies
        for metric, value in metrics.items():
            if value is None or metric not in self.history:
                continue
            
            # Statistical anomaly detection
            anomaly = self.detect_metric_anomaly(metric, value)
            if anomaly:
                anomalies.append(anomaly)
            
            # Rapid change detection
            rapid_change = self.detect_rapid_change(metric, value)
            if rapid_change:
                anomalies.append(rapid_change)
        
        # Sustained degradation detection
        for metric in ['overall_health', 'cpu_temp', 'mem_usage']:
            degradation = self.detect_sustained_degradation(metric)
            if degradation:
                anomalies.append(degradation)
        
        return anomalies
    
    def get_statistics_summary(self) -> Dict:
        """Get summary of all metrics statistics"""
        summary = {}
        
        for metric in self.history.keys():
            stats = self.calculate_statistics(metric)
            if stats:
                mean, stdev = stats
                data = list(self.history[metric])
                summary[metric] = {
                    'mean': mean,
                    'stdev': stdev,
                    'min': min(data),
                    'max': max(data),
                    'current': data[-1] if data else None,
                    'samples': len(data),
                    'anomalies': self.anomaly_count[metric]
                }
        
        return summary


def main():
    """Test anomaly detector"""
    config = {
        'window_size': 20,
        'sensitivity': 2.0
    }
    
    detector = AnomalyDetector(config)
    
    # Simulate normal metrics
    print("Simulating normal metrics...")
    for i in range(15):
        metrics = {
            'cpu_temp': 50.0 + (i % 5),
            'gpu_temp': 48.0 + (i % 4),
            'mem_usage': 60 + (i % 3),
            'cpu_load': 45 + (i % 5),
            'overall_health': 85 - (i % 3)
        }
        detector.detect(metrics)
    
    # Inject anomaly
    print("\nInjecting anomaly...")
    anomalous_metrics = {
        'cpu_temp': 95.0,  # Spike!
        'gpu_temp': 50.0,
        'mem_usage': 95,   # Spike!
        'cpu_load': 50,
        'overall_health': 85
    }
    
    anomalies = detector.detect(anomalous_metrics)
    
    if anomalies:
        print(f"Detected {len(anomalies)} anomalies:")
        for anomaly in anomalies:
            print(f"  - {anomaly['message']}")
    else:
        print("No anomalies detected")
    
    # Print statistics
    print("\nStatistics Summary:")
    summary = detector.get_statistics_summary()
    for metric, stats in summary.items():
        print(f"{metric}: mean={stats['mean']:.2f}, "
              f"stdev={stats['stdev']:.2f}, "
              f"anomalies={stats['anomalies']}")


if __name__ == '__main__':
    main()
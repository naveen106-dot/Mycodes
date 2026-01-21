# Jetson Device Health Monitor

A comprehensive health monitoring system for NVIDIA Jetson Nano devices, featuring a custom kernel module, monitoring daemon, and CLI tools.

## 🌟 Features

- **Real-time Monitoring**: Track CPU, GPU, memory, thermal, and storage metrics
- **Kernel-level Integration**: Custom Linux kernel module for efficient data collection
- **Health Scoring**: Intelligent algorithm that calculates overall device health (0-100)
- **Alert System**: Configurable thresholds with multi-level alerting (Warning/Critical)
- **Anomaly Detection**: Statistical analysis to detect unusual patterns
- **SystemD Integration**: Runs as a system service with automatic startup
- **CLI Tool**: Powerful command-line interface for diagnostics and reporting
- **Data Export**: CSV and JSON export for analysis and integration
- **Low Overhead**: Minimal resource usage (< 5% CPU, < 100MB RAM)

## 📋 Requirements

- **Hardware**: NVIDIA Jetson Nano (2GB or 4GB model)
- **OS**: JetPack 4.x or Ubuntu 18.04/20.04 for Tegra
- **Kernel**: Linux kernel headers matching your running kernel
- **Build Tools**: GCC, Make, Linux headers
- **Python**: Python 3.6 or later
- **Privileges**: Root access for installation

## 🚀 Quick Start

### Installation

```bash
# Clone the repository
git clone https://github.com/yourorg/jetson-device-health-monitor
cd jetson-device-health-monitor

# Run the installation script
sudo ./scripts/install_all.sh
```

The installation script will:
1. Check and install dependencies
2. Build and install the kernel module
3. Install the Python daemon and CLI tool
4. Configure and start the systemd service
5. Set up log rotation and auto-start

### Basic Usage

```bash
# Check current device health
health-diag status

# Generate a detailed report
health-diag report

# Real-time monitoring
health-diag monitor

# View active alerts
health-diag alerts

# Display historical charts
health-diag chart --metric all
```

## 📊 Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    User Space                           │
├─────────────────────────────────────────────────────────┤
│  CLI Tool          │  Monitoring Daemon                 │
│  health-diag       │  device_healthd.py                 │
│                    │                                     │
│  • Status display  │  • Continuous monitoring            │
│  • Report gen      │  • Alert generation                 │
│  • Real-time mon   │  • Anomaly detection                │
│  • Chart display   │  • Data logging                     │
└──────────┬──────────┴──────────────┬───────────────────┘
           │                         │
           │    /sys/kernel/device_health/
           │         (sysfs interface)
           │                         │
┌──────────┴─────────────────────────┴───────────────────┐
│                  Kernel Space                           │
├─────────────────────────────────────────────────────────┤
│             Device Health Kernel Module                 │
│                                                          │
│  ┌────────────┐  ┌─────────────┐  ┌───────────────┐   │
│  │  Thermal   │  │   Memory    │  │      CPU      │   │
│  │  Monitor   │  │   Monitor   │  │    Monitor    │   │
│  └────────────┘  └─────────────┘  └───────────────┘   │
│                                                          │
│  ┌────────────┐  ┌─────────────┐  ┌───────────────┐   │
│  │    GPU     │  │   Storage   │  │    Health     │   │
│  │  Monitor   │  │   Monitor   │  │    Scoring    │   │
│  └────────────┘  └─────────────┘  └───────────────┘   │
└──────────┬───────────────────────────────────────────┘
           │
           │  Hardware access (thermal zones, /proc, etc.)
           │
┌──────────┴──────────────────────────────────────────────┐
│                    Hardware Layer                        │
│  CPU │ GPU │ Memory │ Thermal Sensors │ Storage         │
└─────────────────────────────────────────────────────────┘
```

## 📁 Project Structure

```
jetson-device-health-monitor/
├── kernel-driver/          # Kernel module source code
│   ├── device_health.c     # Main module
│   ├── thermal_monitor.c   # Thermal monitoring
│   ├── memory_monitor.c    # Memory monitoring
│   ├── cpu_monitor.c       # CPU monitoring
│   ├── gpu_monitor.c       # GPU monitoring
│   ├── storage_monitor.c   # Storage monitoring
│   └── health_score.c      # Health scoring algorithm
│
├── daemon/                 # Python monitoring daemon
│   ├── device_healthd.py   # Main daemon
│   ├── sysfs_reader.py     # Sysfs interface
│   ├── logger.py           # Logging system
│   ├── alerting.py         # Alert manager
│   └── anomaly_detector.py # Anomaly detection
│
├── cli-tool/               # Command-line interface
│   ├── health-diag         # Main CLI script
│   ├── report_generator.py # Report formatting
│   └── ascii_charts.py     # Terminal graphs
│
├── systemd/                # SystemD integration
│   └── device-healthd.service
│
├── scripts/                # Utility scripts
│   ├── install_all.sh      # Installation script
│   ├── uninstall_all.sh    # Uninstallation script
│   └── test_stress.sh      # Stress testing
│
├── configs/                # Configuration files
│   └── config.yaml         # Main configuration
│
└── docs/                   # Documentation
    ├── INSTALLATION.md     # Installation guide
    ├── USER_GUIDE.md       # User manual
    └── API_REFERENCE.md    # API documentation
```

## 🔧 Configuration

Edit `/etc/device-health/config.yaml`:

```yaml
monitoring:
  interval: 5               # Update every 5 seconds

alerts:
  enabled: true
  critical_threshold: 30
  warning_threshold: 60
  cooldown: 300            # 5 minutes between alerts

anomaly_detection:
  enabled: true
  window_size: 60
  sensitivity: 2.0

storage:
  export_csv: true
  metrics_retention_days: 30
```

## 📈 Health Scoring Algorithm

The health score (0-100) is calculated using a weighted average:

- **Thermal Health (25%)**: CPU/GPU temperatures
- **Memory Health (20%)**: RAM usage and availability
- **CPU Health (20%)**: CPU load and frequency
- **GPU Health (20%)**: GPU utilization
- **Storage Health (15%)**: Disk usage and I/O

### Score Interpretation

- **80-100**: Excellent - System operating optimally
- **60-79**: Good - Normal operation
- **30-59**: Warning - Attention needed
- **0-29**: Critical - Immediate action required

## 🚨 Alert Thresholds

### Default Thresholds

| Component | Warning | Critical |
|-----------|---------|----------|
| CPU Temp  | 75°C    | 85°C     |
| GPU Temp  | 75°C    | 85°C     |
| Memory    | 85%     | 95%      |
| CPU Load  | 80%     | 95%      |
| GPU Load  | 80%     | 95%      |
| Storage   | 85%     | 95%      |

## 📊 Data Export

Metrics are automatically exported to CSV files:

```
/var/log/device-health/metrics/health_data_YYYYMMDD.csv
```

Format:
```csv
timestamp,overall_health,cpu_temp,gpu_temp,mem_usage,cpu_load,gpu_usage,storage_usage
2024-01-15T10:30:00,85,55.2,52.8,65,45,30,70
```

## 🔍 Troubleshooting

### Module not loading

```bash
# Check kernel logs
dmesg | tail -50

# Verify module file
lsmod | grep device_health

# Manual load
sudo insmod /path/to/device_health.ko
```

### Service not starting

```bash
# Check service status
systemctl status device-healthd

# View logs
journalctl -u device-healthd -n 50

# Test manually
sudo /opt/device-health/device_healthd.py -c /etc/device-health/config.yaml
```

### Sysfs not accessible

```bash
# Verify module is loaded
ls -la /sys/kernel/device_health/

# Check permissions
sudo chmod 755 /sys/kernel/device_health/
```

## 🧪 Testing

```bash
# Run unit tests
cd tests
./test_kernel_module.sh
python3 test_daemon.py

# Stress test
./scripts/test_stress.sh

# Integration test
./tests/integration_test.sh
```

## 🔒 Security

- Kernel module runs with kernel privileges
- Daemon runs as root (required for hardware access)
- Sysfs interface has restricted permissions
- No network exposure by default
- Logs contain only metrics, no sensitive data

## 📝 Logging

Logs are stored in `/var/log/device-health/`:

- `daemon.log`: Daemon operation logs
- `alerts.log`: Alert history
- `errors.log`: Error messages
- `metrics/`: Historical metrics (CSV)

Log rotation is automatic (daily, keep 7 days).

## 🤝 Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## 📄 License

This project is licensed under the GPL v2 License - see [LICENSE](LICENSE) file.

## 🙏 Acknowledgments

- NVIDIA for Jetson platform
- Linux kernel community
- Python community

## 📞 Support

- Issues: [GitHub Issues](https://github.com/yourorg/jetson-device-health-monitor/issues)
- Documentation: [Wiki](https://github.com/yourorg/jetson-device-health-monitor/wiki)
- Email: support@example.com

## 🗺️ Roadmap

- [ ] Web dashboard interface
- [ ] Email/SMS notifications
- [ ] Multi-device monitoring
- [ ] Machine learning predictions
- [ ] Docker container support
- [ ] Jetson Xavier/Orin support

---

**Made with ❤️ for the Jetson community**
# Makefile for Device Health Monitor Kernel Module

obj-m += device_health.o

device_health-objs := \
	device_health_main.o \
	thermal_monitor.o \
	memory_monitor.o \
	cpu_monitor.o \
	gpu_monitor.o \
	storage_monitor.o \
	health_score.o

# Kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# Default target
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean build artifacts
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f Module.symvers modules.order

# Install module
install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
	depmod -a

# Uninstall module
uninstall:
	rmmod device_health 2>/dev/null || true
	rm -f /lib/modules/$(shell uname -r)/extra/device_health.ko
	depmod -a

# Load module
load:
	insmod device_health.ko

# Unload module
unload:
	rmmod device_health

# Show module info
info:
	modinfo device_health.ko

# View kernel logs
log:
	dmesg | tail -50 | grep "Device Health"

# Test module
test: all load
	@echo "Module loaded successfully"
	@ls -la /sys/kernel/device_health/
	@cat /sys/kernel/device_health/full_report

.PHONY: all clean install uninstall load unload info log test
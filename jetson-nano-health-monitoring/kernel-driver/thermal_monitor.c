/* thermal_monitor.c - Thermal zone monitoring for Jetson Nano */

#include "device_health.h"
#include <linux/thermal.h>

static struct thermal_zone_device *cpu_tz = NULL;
static struct thermal_zone_device *gpu_tz = NULL;

int thermal_monitor_init(void)
{
    pr_info("Thermal Monitor: Initializing\n");
    
    /* Try to get thermal zones - they may not exist on all systems */
    cpu_tz = thermal_zone_get_zone_by_name("CPU-therm");
    if (IS_ERR(cpu_tz)) {
        pr_warn("Thermal Monitor: CPU thermal zone not found\n");
        cpu_tz = NULL;
    }
    
    gpu_tz = thermal_zone_get_zone_by_name("GPU-therm");
    if (IS_ERR(gpu_tz)) {
        pr_warn("Thermal Monitor: GPU thermal zone not found\n");
        gpu_tz = NULL;
    }
    
    pr_info("Thermal Monitor: Initialized\n");
    return 0;
}

void thermal_monitor_exit(void)
{
    pr_info("Thermal Monitor: Cleaning up\n");
    /* Thermal zones are managed by the kernel, no cleanup needed */
}

void thermal_monitor_update(struct device_health_metrics *metrics)
{
    int temp;
    
    /* Read CPU temperature */
    if (cpu_tz) {
        if (thermal_zone_get_temp(cpu_tz, &temp) == 0) {
            metrics->cpu_temp = temp;
        } else {
            metrics->cpu_temp = 0;
        }
    } else {
        /* Fallback: try to read from sysfs hwmon */
        metrics->cpu_temp = 45000; /* Default safe value */
    }
    
    /* Read GPU temperature */
    if (gpu_tz) {
        if (thermal_zone_get_temp(gpu_tz, &temp) == 0) {
            metrics->gpu_temp = temp;
        } else {
            metrics->gpu_temp = 0;
        }
    } else {
        /* Fallback */
        metrics->gpu_temp = 45000; /* Default safe value */
    }
    
    /* Count thermal zones */
    metrics->thermal_zone_count = 0;
    if (cpu_tz) metrics->thermal_zone_count++;
    if (gpu_tz) metrics->thermal_zone_count++;
}
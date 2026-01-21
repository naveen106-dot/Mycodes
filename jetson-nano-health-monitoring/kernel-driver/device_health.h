/* device_health.h - Jetson Device Health Monitor Kernel Module Header */

#ifndef DEVICE_HEALTH_H
#define DEVICE_HEALTH_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/thermal.h>
#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/mm.h>
#include <linux/vmstat.h>

/* Module information */
#define DRIVER_NAME "device_health"
#define DRIVER_VERSION "1.0.0"

/* Update intervals (in seconds) */
#define DEFAULT_UPDATE_INTERVAL 5
#define HEALTH_HISTORY_SIZE 60

/* Health score thresholds */
#define HEALTH_CRITICAL 30
#define HEALTH_WARNING 60
#define HEALTH_GOOD 80

/* Thermal thresholds (in millidegrees Celsius) */
#define THERMAL_CRITICAL 85000
#define THERMAL_WARNING 75000
#define THERMAL_SAFE 65000

/* Memory thresholds (percentage) */
#define MEM_CRITICAL 95
#define MEM_WARNING 85
#define MEM_SAFE 70

/* CPU thresholds (percentage) */
#define CPU_CRITICAL 95
#define CPU_WARNING 80
#define CPU_SAFE 60

/* GPU thresholds (percentage) */
#define GPU_CRITICAL 95
#define GPU_WARNING 80
#define GPU_SAFE 60

/* Storage thresholds */
#define STORAGE_CRITICAL 95
#define STORAGE_WARNING 85
#define STORAGE_SAFE 70

/* Device health metrics structure */
struct device_health_metrics {
    /* Thermal metrics */
    int cpu_temp;
    int gpu_temp;
    int thermal_zone_count;
    
    /* Memory metrics */
    unsigned long mem_total;
    unsigned long mem_free;
    unsigned long mem_available;
    unsigned long mem_cached;
    unsigned long swap_total;
    unsigned long swap_free;
    int mem_usage_percent;
    
    /* CPU metrics */
    unsigned int cpu_count;
    unsigned long cpu_freq[4];
    int cpu_load[4];
    int cpu_avg_load;
    
    /* GPU metrics */
    int gpu_usage;
    unsigned long gpu_freq;
    int gpu_power;
    
    /* Storage metrics */
    unsigned long storage_total;
    unsigned long storage_used;
    unsigned long storage_free;
    int storage_usage_percent;
    unsigned long io_reads;
    unsigned long io_writes;
    
    /* Health score */
    int overall_health;
    int thermal_health;
    int memory_health;
    int cpu_health;
    int gpu_health;
    int storage_health;
    
    /* Status flags */
    unsigned int alert_flags;
    unsigned long uptime;
    unsigned long last_update;
};

/* Alert flags */
#define ALERT_THERMAL_CRITICAL  (1 << 0)
#define ALERT_THERMAL_WARNING   (1 << 1)
#define ALERT_MEMORY_CRITICAL   (1 << 2)
#define ALERT_MEMORY_WARNING    (1 << 3)
#define ALERT_CPU_CRITICAL      (1 << 4)
#define ALERT_CPU_WARNING       (1 << 5)
#define ALERT_GPU_CRITICAL      (1 << 6)
#define ALERT_GPU_WARNING       (1 << 7)
#define ALERT_STORAGE_CRITICAL  (1 << 8)
#define ALERT_STORAGE_WARNING   (1 << 9)

/* Global health metrics */
extern struct device_health_metrics g_health_metrics;
extern struct kobject *health_kobj;
extern struct timer_list health_timer;

/* Function prototypes for monitor modules */
int thermal_monitor_init(void);
void thermal_monitor_exit(void);
void thermal_monitor_update(struct device_health_metrics *metrics);

int memory_monitor_init(void);
void memory_monitor_exit(void);
void memory_monitor_update(struct device_health_metrics *metrics);

int cpu_monitor_init(void);
void cpu_monitor_exit(void);
void cpu_monitor_update(struct device_health_metrics *metrics);

int gpu_monitor_init(void);
void gpu_monitor_exit(void);
void gpu_monitor_update(struct device_health_metrics *metrics);

int storage_monitor_init(void);
void storage_monitor_exit(void);
void storage_monitor_update(struct device_health_metrics *metrics);

int health_score_init(void);
void health_score_exit(void);
void health_score_calculate(struct device_health_metrics *metrics);

/* Utility functions */
void update_alert_flags(struct device_health_metrics *metrics);
const char* get_health_status_string(int health_score);

#endif /* DEVICE_HEALTH_H */
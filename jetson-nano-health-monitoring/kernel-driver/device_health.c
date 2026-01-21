/* device_health.c - Main Jetson Device Health Monitor Kernel Module */

#include "device_health.h"
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/timekeeping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jetson Health Monitor");
MODULE_DESCRIPTION("Device Health Monitoring Driver for NVIDIA Jetson Nano");
MODULE_VERSION(DRIVER_VERSION);

/* Global variables */
struct device_health_metrics g_health_metrics;
struct kobject *health_kobj;
struct timer_list health_timer;
static unsigned int update_interval = DEFAULT_UPDATE_INTERVAL;

/* Sysfs show functions */
static ssize_t overall_health_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_health_metrics.overall_health);
}

static ssize_t cpu_temp_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_health_metrics.cpu_temp);
}

static ssize_t gpu_temp_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_health_metrics.gpu_temp);
}

static ssize_t mem_usage_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_health_metrics.mem_usage_percent);
}

static ssize_t cpu_load_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_health_metrics.cpu_avg_load);
}

static ssize_t gpu_usage_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_health_metrics.gpu_usage);
}

static ssize_t storage_usage_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", g_health_metrics.storage_usage_percent);
}

static ssize_t alert_flags_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "0x%08x\n", g_health_metrics.alert_flags);
}

static ssize_t health_status_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%s\n", get_health_status_string(g_health_metrics.overall_health));
}

static ssize_t full_report_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf,
        "=== Jetson Device Health Report ===\n"
        "Overall Health: %d (%s)\n"
        "Uptime: %lu seconds\n\n"
        "Thermal:\n"
        "  CPU Temp: %d mC\n"
        "  GPU Temp: %d mC\n"
        "  Health Score: %d\n\n"
        "Memory:\n"
        "  Total: %lu KB\n"
        "  Free: %lu KB\n"
        "  Available: %lu KB\n"
        "  Usage: %d%%\n"
        "  Health Score: %d\n\n"
        "CPU:\n"
        "  Count: %u\n"
        "  Average Load: %d%%\n"
        "  Health Score: %d\n\n"
        "GPU:\n"
        "  Usage: %d%%\n"
        "  Frequency: %lu MHz\n"
        "  Health Score: %d\n\n"
        "Storage:\n"
        "  Total: %lu KB\n"
        "  Used: %lu KB\n"
        "  Free: %lu KB\n"
        "  Usage: %d%%\n"
        "  Health Score: %d\n\n"
        "Alert Flags: 0x%08x\n",
        g_health_metrics.overall_health,
        get_health_status_string(g_health_metrics.overall_health),
        g_health_metrics.uptime,
        g_health_metrics.cpu_temp,
        g_health_metrics.gpu_temp,
        g_health_metrics.thermal_health,
        g_health_metrics.mem_total,
        g_health_metrics.mem_free,
        g_health_metrics.mem_available,
        g_health_metrics.mem_usage_percent,
        g_health_metrics.memory_health,
        g_health_metrics.cpu_count,
        g_health_metrics.cpu_avg_load,
        g_health_metrics.cpu_health,
        g_health_metrics.gpu_usage,
        g_health_metrics.gpu_freq,
        g_health_metrics.gpu_health,
        g_health_metrics.storage_total,
        g_health_metrics.storage_used,
        g_health_metrics.storage_free,
        g_health_metrics.storage_usage_percent,
        g_health_metrics.storage_health,
        g_health_metrics.alert_flags);
}

static ssize_t update_interval_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%u\n", update_interval);
}

static ssize_t update_interval_store(struct kobject *kobj, struct kobj_attribute *attr,
                                     const char *buf, size_t count)
{
    unsigned int val;
    if (kstrtouint(buf, 10, &val) == 0 && val > 0 && val <= 60) {
        update_interval = val;
        mod_timer(&health_timer, jiffies + msecs_to_jiffies(update_interval * 1000));
        return count;
    }
    return -EINVAL;
}

/* Sysfs attributes */
static struct kobj_attribute overall_health_attr = __ATTR_RO(overall_health);
static struct kobj_attribute cpu_temp_attr = __ATTR_RO(cpu_temp);
static struct kobj_attribute gpu_temp_attr = __ATTR_RO(gpu_temp);
static struct kobj_attribute mem_usage_attr = __ATTR_RO(mem_usage);
static struct kobj_attribute cpu_load_attr = __ATTR_RO(cpu_load);
static struct kobj_attribute gpu_usage_attr = __ATTR_RO(gpu_usage);
static struct kobj_attribute storage_usage_attr = __ATTR_RO(storage_usage);
static struct kobj_attribute alert_flags_attr = __ATTR_RO(alert_flags);
static struct kobj_attribute health_status_attr = __ATTR_RO(health_status);
static struct kobj_attribute full_report_attr = __ATTR_RO(full_report);
static struct kobj_attribute update_interval_attr = __ATTR_RW(update_interval);

static struct attribute *health_attrs[] = {
    &overall_health_attr.attr,
    &cpu_temp_attr.attr,
    &gpu_temp_attr.attr,
    &mem_usage_attr.attr,
    &cpu_load_attr.attr,
    &gpu_usage_attr.attr,
    &storage_usage_attr.attr,
    &alert_flags_attr.attr,
    &health_status_attr.attr,
    &full_report_attr.attr,
    &update_interval_attr.attr,
    NULL,
};

static struct attribute_group health_attr_group = {
    .attrs = health_attrs,
};

/* Utility functions */
const char* get_health_status_string(int health_score)
{
    if (health_score >= HEALTH_GOOD)
        return "EXCELLENT";
    else if (health_score >= HEALTH_WARNING)
        return "GOOD";
    else if (health_score >= HEALTH_CRITICAL)
        return "WARNING";
    else
        return "CRITICAL";
}

void update_alert_flags(struct device_health_metrics *metrics)
{
    metrics->alert_flags = 0;
    
    /* Thermal alerts */
    if (metrics->cpu_temp >= THERMAL_CRITICAL || metrics->gpu_temp >= THERMAL_CRITICAL)
        metrics->alert_flags |= ALERT_THERMAL_CRITICAL;
    else if (metrics->cpu_temp >= THERMAL_WARNING || metrics->gpu_temp >= THERMAL_WARNING)
        metrics->alert_flags |= ALERT_THERMAL_WARNING;
    
    /* Memory alerts */
    if (metrics->mem_usage_percent >= MEM_CRITICAL)
        metrics->alert_flags |= ALERT_MEMORY_CRITICAL;
    else if (metrics->mem_usage_percent >= MEM_WARNING)
        metrics->alert_flags |= ALERT_MEMORY_WARNING;
    
    /* CPU alerts */
    if (metrics->cpu_avg_load >= CPU_CRITICAL)
        metrics->alert_flags |= ALERT_CPU_CRITICAL;
    else if (metrics->cpu_avg_load >= CPU_WARNING)
        metrics->alert_flags |= ALERT_CPU_WARNING;
    
    /* GPU alerts */
    if (metrics->gpu_usage >= GPU_CRITICAL)
        metrics->alert_flags |= ALERT_GPU_CRITICAL;
    else if (metrics->gpu_usage >= GPU_WARNING)
        metrics->alert_flags |= ALERT_GPU_WARNING;
    
    /* Storage alerts */
    if (metrics->storage_usage_percent >= STORAGE_CRITICAL)
        metrics->alert_flags |= ALERT_STORAGE_CRITICAL;
    else if (metrics->storage_usage_percent >= STORAGE_WARNING)
        metrics->alert_flags |= ALERT_STORAGE_WARNING;
}

/* Health monitoring timer callback */
static void health_timer_callback(struct timer_list *timer)
{
    /* Update all metrics */
    thermal_monitor_update(&g_health_metrics);
    memory_monitor_update(&g_health_metrics);
    cpu_monitor_update(&g_health_metrics);
    gpu_monitor_update(&g_health_metrics);
    storage_monitor_update(&g_health_metrics);
    
    /* Calculate health scores */
    health_score_calculate(&g_health_metrics);
    
    /* Update alert flags */
    update_alert_flags(&g_health_metrics);
    
    /* Update timestamp */
    g_health_metrics.last_update = jiffies;
    g_health_metrics.uptime = get_seconds();
    
    /* Reschedule timer */
    mod_timer(&health_timer, jiffies + msecs_to_jiffies(update_interval * 1000));
}

/* Module initialization */
static int __init device_health_init(void)
{
    int ret;
    
    pr_info("Device Health Monitor: Initializing v%s\n", DRIVER_VERSION);
    
    /* Initialize metrics structure */
    memset(&g_health_metrics, 0, sizeof(g_health_metrics));
    
    /* Create sysfs kobject */
    health_kobj = kobject_create_and_add("device_health", kernel_kobj);
    if (!health_kobj) {
        pr_err("Device Health Monitor: Failed to create kobject\n");
        return -ENOMEM;
    }
    
    /* Create sysfs group */
    ret = sysfs_create_group(health_kobj, &health_attr_group);
    if (ret) {
        pr_err("Device Health Monitor: Failed to create sysfs group\n");
        kobject_put(health_kobj);
        return ret;
    }
    
    /* Initialize monitor modules */
    ret = thermal_monitor_init();
    if (ret) goto err_thermal;
    
    ret = memory_monitor_init();
    if (ret) goto err_memory;
    
    ret = cpu_monitor_init();
    if (ret) goto err_cpu;
    
    ret = gpu_monitor_init();
    if (ret) goto err_gpu;
    
    ret = storage_monitor_init();
    if (ret) goto err_storage;
    
    ret = health_score_init();
    if (ret) goto err_health_score;
    
    /* Setup and start timer */
    timer_setup(&health_timer, health_timer_callback, 0);
    mod_timer(&health_timer, jiffies + msecs_to_jiffies(update_interval * 1000));
    
    pr_info("Device Health Monitor: Initialized successfully\n");
    return 0;

err_health_score:
    storage_monitor_exit();
err_storage:
    gpu_monitor_exit();
err_gpu:
    cpu_monitor_exit();
err_cpu:
    memory_monitor_exit();
err_memory:
    thermal_monitor_exit();
err_thermal:
    sysfs_remove_group(health_kobj, &health_attr_group);
    kobject_put(health_kobj);
    return ret;
}

/* Module cleanup */
static void __exit device_health_exit(void)
{
    pr_info("Device Health Monitor: Cleaning up\n");
    
    /* Stop timer */
    del_timer_sync(&health_timer);
    
    /* Cleanup monitor modules */
    health_score_exit();
    storage_monitor_exit();
    gpu_monitor_exit();
    cpu_monitor_exit();
    memory_monitor_exit();
    thermal_monitor_exit();
    
    /* Remove sysfs entries */
    sysfs_remove_group(health_kobj, &health_attr_group);
    kobject_put(health_kobj);
    
    pr_info("Device Health Monitor: Unloaded\n");
}

module_init(device_health_init);
module_exit(device_health_exit);
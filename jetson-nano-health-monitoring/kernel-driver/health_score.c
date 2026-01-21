/* health_score.c - Health scoring algorithm for Jetson Nano */

#include "device_health.h"

int health_score_init(void)
{
    pr_info("Health Score: Initializing\n");
    return 0;
}

void health_score_exit(void)
{
    pr_info("Health Score: Cleaning up\n");
}

static int calculate_thermal_health(struct device_health_metrics *metrics)
{
    int max_temp, score;
    
    /* Find maximum temperature */
    max_temp = (metrics->cpu_temp > metrics->gpu_temp) ? 
                metrics->cpu_temp : metrics->gpu_temp;
    
    /* Calculate score based on temperature */
    if (max_temp >= THERMAL_CRITICAL) {
        score = 0;
    } else if (max_temp >= THERMAL_WARNING) {
        /* Linear interpolation between WARNING and CRITICAL */
        score = 50 - ((max_temp - THERMAL_WARNING) * 50) / 
                (THERMAL_CRITICAL - THERMAL_WARNING);
    } else if (max_temp >= THERMAL_SAFE) {
        /* Linear interpolation between SAFE and WARNING */
        score = 80 - ((max_temp - THERMAL_SAFE) * 30) / 
                (THERMAL_WARNING - THERMAL_SAFE);
    } else {
        /* Optimal temperature range */
        score = 100;
    }
    
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    
    return score;
}

static int calculate_memory_health(struct device_health_metrics *metrics)
{
    int usage = metrics->mem_usage_percent;
    int score;
    
    if (usage >= MEM_CRITICAL) {
        score = 10;
    } else if (usage >= MEM_WARNING) {
        /* Linear interpolation */
        score = 50 - ((usage - MEM_WARNING) * 40) / (MEM_CRITICAL - MEM_WARNING);
    } else if (usage >= MEM_SAFE) {
        score = 80 - ((usage - MEM_SAFE) * 30) / (MEM_WARNING - MEM_SAFE);
    } else {
        score = 100;
    }
    
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    
    return score;
}

static int calculate_cpu_health(struct device_health_metrics *metrics)
{
    int load = metrics->cpu_avg_load;
    int score;
    
    if (load >= CPU_CRITICAL) {
        score = 10;
    } else if (load >= CPU_WARNING) {
        score = 50 - ((load - CPU_WARNING) * 40) / (CPU_CRITICAL - CPU_WARNING);
    } else if (load >= CPU_SAFE) {
        score = 80 - ((load - CPU_SAFE) * 30) / (CPU_WARNING - CPU_SAFE);
    } else {
        score = 100;
    }
    
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    
    return score;
}

static int calculate_gpu_health(struct device_health_metrics *metrics)
{
    int usage = metrics->gpu_usage;
    int score;
    
    if (usage >= GPU_CRITICAL) {
        score = 10;
    } else if (usage >= GPU_WARNING) {
        score = 50 - ((usage - GPU_WARNING) * 40) / (GPU_CRITICAL - GPU_WARNING);
    } else if (usage >= GPU_SAFE) {
        score = 80 - ((usage - GPU_SAFE) * 30) / (GPU_WARNING - GPU_SAFE);
    } else {
        score = 100;
    }
    
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    
    return score;
}

static int calculate_storage_health(struct device_health_metrics *metrics)
{
    int usage = metrics->storage_usage_percent;
    int score;
    
    if (usage >= STORAGE_CRITICAL) {
        score = 10;
    } else if (usage >= STORAGE_WARNING) {
        score = 50 - ((usage - STORAGE_WARNING) * 40) / 
                (STORAGE_CRITICAL - STORAGE_WARNING);
    } else if (usage >= STORAGE_SAFE) {
        score = 80 - ((usage - STORAGE_SAFE) * 30) / 
                (STORAGE_WARNING - STORAGE_SAFE);
    } else {
        score = 100;
    }
    
    if (score < 0) score = 0;
    if (score > 100) score = 100;
    
    return score;
}

void health_score_calculate(struct device_health_metrics *metrics)
{
    int thermal_health, memory_health, cpu_health, gpu_health, storage_health;
    int overall_health;
    
    /* Calculate individual health scores */
    thermal_health = calculate_thermal_health(metrics);
    memory_health = calculate_memory_health(metrics);
    cpu_health = calculate_cpu_health(metrics);
    gpu_health = calculate_gpu_health(metrics);
    storage_health = calculate_storage_health(metrics);
    
    /* Store individual scores */
    metrics->thermal_health = thermal_health;
    metrics->memory_health = memory_health;
    metrics->cpu_health = cpu_health;
    metrics->gpu_health = gpu_health;
    metrics->storage_health = storage_health;
    
    /* Calculate overall health with weighted average */
    /* Weights: thermal=25%, memory=20%, cpu=20%, gpu=20%, storage=15% */
    overall_health = (thermal_health * 25 + 
                     memory_health * 20 + 
                     cpu_health * 20 + 
                     gpu_health * 20 + 
                     storage_health * 15) / 100;
    
    /* Apply penalty for critical conditions */
    if (thermal_health < 20 || memory_health < 20) {
        overall_health = (overall_health < 30) ? overall_health : 30;
    }
    
    metrics->overall_health = overall_health;
}
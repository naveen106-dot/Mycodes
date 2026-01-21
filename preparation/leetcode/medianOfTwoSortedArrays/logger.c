#include "logger.h"
#include <stdarg.h>
#include <string.h>
#include <time.h>

static FILE *log_file = NULL;
static LogLevel current_level = LOG_DEBUG;

static const char *level_to_str(LogLevel level) {
    switch (level) {
        case LOG_DEBUG: return "DEBUG";
        case LOG_INFO:  return "INFO";
        case LOG_WARN:  return "WARN";
        case LOG_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int logger_init(const char *filename, LogLevel level) {
    current_level = level;
    log_file = fopen(filename, "a");

    if (!log_file) return -1;

    INFO("Logger initialized.");
    return 0;
}

void logger_close() {
    if (log_file) {
        INFO("Logger closed.");
        fclose(log_file);
        log_file = NULL;
    }
}



void logger_log(LogLevel level, const char *file, int line, const char *fmt, ...) {
    
    

    // Print to stdout
    printf("%s [%s] (%s:%d): ", level_to_str(level), file, line);


    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);   // <-- use vprintf instead of vfprintf
    va_end(args);

    printf("\n");
    fflush(stdout);
}


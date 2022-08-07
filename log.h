#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include <time.h>

enum log_level {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

void logger_init(char *outp_path, enum log_level level);
void logger_free();

void logger_vlog(
    enum log_level level,
    char *file,
    char *func,
    size_t line,
    char *fmt,
    va_list args
);

void logger_log(
    enum log_level level,
    char *file,
    char *func,
    size_t line,
    char *fmt,
    ...
);

#define log_msg(level, ...) \
    logger_log(level, (char *)__FILE__, (char *)__func__, __LINE__, __VA_ARGS__)

#define log_trace(...) log_msg(LOG_TRACE, __VA_ARGS__)
#define log_debug(...) log_msg(LOG_DEBUG, __VA_ARGS__)
#define log_info(...) log_msg(LOG_INFO, __VA_ARGS__)
#define log_warn(...) log_msg(LOG_WARNING, __VA_ARGS__)
#define log_error(...) log_msg(LOG_ERROR, __VA_ARGS__)
#pragma once

#include <log.h>

#include <stdarg.h>
#include <stdlib.h>

void die(char *where);
void vdie_with(char *where, char *fmt, va_list args);
void die_with(char *where, char *fmt, ...);

#define die_fancy(...) (log_error(__VA_ARGS__), exit(1))
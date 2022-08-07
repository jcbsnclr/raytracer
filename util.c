#include <util.h>
#include <log.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void die(char *where) {
    perror(where);
    exit(1);
}

void vdie_with(char *where, char *fmt, va_list args) {
    fprintf(stderr, "%s: ", where);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);

    exit(1);
}

void die_with(char *where, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vdie_with(where, fmt, args);
}
#include <log.h>
#include <util.h>

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

static FILE *outp;
static enum log_level filter = LOG_INFO;

static char *level_table[] = {
    [LOG_TRACE] = "trace",
    [LOG_DEBUG] = "debug",
    [LOG_INFO] = "info",
    [LOG_WARNING] = "warning",
    [LOG_ERROR] = "error"
};

void logger_init(char *outp_path, enum log_level level) {
    filter = level;

    // if no path is provided, don't write to file
    if (outp_path) {
        // open output file for writing, create if doesn't exist
        outp = fopen(outp_path, "w+");

        if (!outp)
            die("logger_init");
    } else {
        outp = NULL;
    }
}

void logger_free() {
    if (fclose(outp) != 0)
        die("logger_free");
}

void logger_vlog(
    enum log_level level,
    char *file,
    char *func,
    size_t line,
    char *fmt,
    va_list args
) {
    assert(file != NULL);
    assert(func != NULL);
    assert(fmt != NULL);

    // get timestamp as YYYY-MM-DD hh:mm
    time_t cur_time = time(NULL);

    if (cur_time == (time_t)-1)
        die("logger_vlog");

    struct tm *tinfo = localtime(&cur_time);
    char buf[17];

    strftime(buf, sizeof(buf), "%F %R", tinfo);
    buf[16] = 0;

    // log message to temporary buffer
    char *tmp_buf;
    size_t tmp_buf_len;

    FILE *tmp_stream = open_memstream(&tmp_buf, &tmp_buf_len);

    if (!tmp_stream)
        die("logger_vlog");

    fprintf(
        tmp_stream, "[%s] <%s @ %s:%lu> %s: ", 
        buf, func, file, line, level_table[level]
    );

    vfprintf(tmp_stream, fmt, args);
    fputc('\n', tmp_stream);

    // flush and close stream; buffer remains
    fclose(tmp_stream);

    // hide verbose messages from stderr
    if (level >= filter)
        fwrite(tmp_buf, 1, tmp_buf_len, stderr);
    
    // write message to file
    if (outp)
        fwrite(tmp_buf, 1, tmp_buf_len, outp);

    // free temporary buffer
    free(tmp_buf);
}

void logger_log(
    enum log_level level,
    char *file,
    char *func,
    size_t line,
    char *fmt,
    ...
) {
    va_list args;
    va_start(args, fmt);

    logger_vlog(level, file, func, line, fmt, args);

    va_end(args);
}
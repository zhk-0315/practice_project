#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "logout.h"

static _Bool g_clear_log_file = 0;
static long g_max_log_size = 0;

void init_log_file_clear(_Bool init_clear_file, long max_log_size)
{
    g_clear_log_file = init_clear_file;
    g_max_log_size = max_log_size;
}

static void output_time_stamp(FILE* fp)
{
    struct timeval tv_ = { 0 };
    struct tm tm_ = { 0 };

    gettimeofday(&tv_, NULL);
    localtime_r(&tv_.tv_sec, &tm_);

    fprintf(fp, "(%04d-%02d-%02d %02d:%02d:%06.03f)", tm_.tm_year + 1900, tm_.tm_mon + 1, tm_.tm_mday,
        tm_.tm_hour, tm_.tm_min, tm_.tm_sec + tv_.tv_usec / 1000000.0);
}

void __logout(const log_file_t* file, const char* fmt, ...)
{
    static FILE* fp = NULL;
    va_list ap = { 0 };

    if (!fp) {
        fp = fopen(file->path, "a+");
    }

    if (!fp) {
        return;
    }

    if (g_clear_log_file
        || (g_max_log_size > 0 && ftell(fp) >= g_max_log_size)) {
        printf("clear log file=============");
        freopen(file->path, "w+", fp);
        g_clear_log_file = 0;
    }

    fflush(stdout);

    output_time_stamp(fp);

    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);

    fflush(fp);
}
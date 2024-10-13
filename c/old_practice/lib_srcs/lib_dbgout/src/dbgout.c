#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "dbgout.h"

#if 0
#define LCPRINTF(fmt, args...) \
    do {                       \
        printf(fmt, ##args);   \
    } while (0)
#else
#define LCPRINTF(fmt, args...)
#endif

#define FPRINTF(fp, fmt, args...)         \
    do {                                  \
        if (fprintf(fp, fmt, ##args) < 0) \
            perror("fprintf error!");     \
        fflush(fp);                       \
    } while (0)

static _Bool initLogFile = 0;

void InitLogFile(void)
{
    initLogFile = 1;

    return;
}

static int GetTimer(char* timeStr)
{
    int ret = 0;
    struct timeval tv = { 0 };
    struct tm* tm = NULL;

    ret = gettimeofday(&tv, NULL);
    if (ret < 0) {
        return ret;
    }

    tm = localtime(&tv.tv_sec);
    sprintf(timeStr, "%04d-%02d-%02d,%02d:%02d:%06.03f", 1900 + tm->tm_year,
        1 + tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min,
        tm->tm_sec + tv.tv_usec / 1000000.0);

    return 0;
}

// static void clearLogFile(LogOp* logOp)
// {
//     freopen(logOp->filePath, "w", logOp->logFp);
//     freopen(logOp->filePath, "a+", logOp->logFp);
// }

static void LogFileFopen(LogOp* logOp)
{
    if (logOp->logFp == NULL) {
        LCPRINTF("fopen %s\n", logOp->filePath);
        logOp->logFp = fopen(logOp->filePath, "a+");
        if (logOp->logFp == NULL) {
            LCPRINTF("fopen logFp error\n");
        }
    }

    if (initLogFile) {
        freopen(logOp->filePath, "w+", logOp->logFp);
        initLogFile = 0;
    }
}

void LogFileFclose(LogOp* logOp)
{
    if (logOp->logFp != NULL) {
        LCPRINTF("fclose\n");
        fclose(logOp->logFp);
        logOp->logFp = NULL;
    }
}

void _dbgout(LogOp* logOp, const char* fmt, ...)
{
    char str[1024] = { 0 };
    char timeStr[128] = { 0 };
    va_list ap;

    GetTimer(timeStr);
    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    if (logOp->logType == ALWAYS_OPEN) {
        LogFileFopen(logOp);
        FPRINTF(logOp->logFp, "%s-%s\n", timeStr, str);
        LCPRINTF("ao->%s-%s\n", timeStr, str);
    } else if (logOp->logType == OPEN_EVERY_TIME_WRITE) {
        LogFileFopen(logOp);
        FPRINTF(logOp->logFp, "%s-%s\n", timeStr, str);
        LCPRINTF("oe->%s-%s\n", timeStr, str);
        LogFileFclose(logOp);
    }
}

const char* NodirStr(const char* str)
{
    static char* tmp = NULL;

    tmp = strrchr(str, '/');

    return (tmp != NULL) ? tmp + 1 : str;
}

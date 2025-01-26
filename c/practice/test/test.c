#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#if 0
int DisplayTime(void)
{
    int iRet = 0;
    char str[1024] = { 0 };
    struct timeval _tp = { 0 };
    struct tm _tm_temp = { 0 };
    struct tm* tmptr = NULL;

    iRet = gettimeofday(&_tp, NULL);
    if (iRet)
        perror("gettimeofday");

    tmptr = localtime_r(&_tp.tv_sec, &_tm_temp);
    if (tmptr != &_tm_temp)
        perror("localtime_r");

    sprintf(str, "%02d-%02d-> %02d:%02d:%02d.%.03d",
        _tm_temp.tm_mon + 1, _tm_temp.tm_mday, _tm_temp.tm_hour,
        _tm_temp.tm_min, _tm_temp.tm_sec, (int)(_tp.tv_usec / 1000));

    printf("%s!\n", str);
    fflush(stdout);

    return 0;
}
#else
int DisplayTime(void)
{
    int iRet = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    char str[1024] = { 0 };
    struct timespec _tp = { 0 };

    iRet = clock_gettime(CLOCK_MONOTONIC_COARSE, &_tp);

    min = _tp.tv_sec / 60;
    sec = _tp.tv_sec % 60;
    hour = min / 60;
    min = min % 60;
    day = hour / 24;
    hour = hour % 24;

    sprintf(str, "poweron-day-%02d -> %02d:%02d:%02d.%03ld", day, hour, min, sec, _tp.tv_nsec / 1000000);

    printf("%s;\n", str);
    fflush(stdout);

    return 0;
}
#endif

int main(void)
{
    int p = 0;

    while (p++ < 10) {
        DisplayTime();
        usleep(300000);
    }

    return 0;
}

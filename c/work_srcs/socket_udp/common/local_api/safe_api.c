#ifdef SAFE_API

#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <pthread.h>
#include <string.h>

char* LCstrcasestr(const char* haystack, const char* needle)
{
    return strcasestr(haystack, needle);
}

int LCpthread_setname_np(pthread_t thread, const char* name)
{
    return pthread_setname_np(thread, name);
}

int LCpthread_getname_np(pthread_t thread, char* name, size_t size)
{
    return pthread_getname_np(thread, name, size);
}

#endif
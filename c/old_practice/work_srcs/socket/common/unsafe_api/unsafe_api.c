#ifdef UNSAFE_API
#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <pthread.h>
#include <string.h>

int lc_pthread_setname_np(pthread_t thread, const char* name)
{
    return pthread_setname_np(thread, name);
}

int lc_pthread_getname_np(pthread_t thread, char name[], size_t size)
{
    return pthread_getname_np(thread, name, size);
}

char* lc_strcasestr(const char* haystack, const char* needle)
{
    return strcasestr(haystack, needle);
}

#endif
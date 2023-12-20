#ifdef SOCKET_TCP_SAFE_API

#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <stdio.h>
#include <string.h>

char* SafeStrcasestr(const char* haystack, const char* needle)
{
    return strcasestr(haystack, needle);
}

//
//
//
#endif
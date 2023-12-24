#ifndef __COMMON__UNSAFE_API__UNSAFE_API_H
#define __COMMON__UNSAFE_API__UNSAFE_API_H

#ifdef UNSAFE_API

#include <pthread.h>

int lc_pthread_setname_np(pthread_t thread, const char* name);
int lc_pthread_getname_np(pthread_t thread, char name[], size_t size);
char* lc_strcasestr(const char* haystack, const char* needle);

#endif

#endif
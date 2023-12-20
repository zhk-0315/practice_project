#ifndef COMMON__SAFE_API__SAFE_API_H
#define COMMON__SAFE_API__SAFE_API_H

#ifdef SAFE_API

#include <pthread.h>

char* LCstrcasestr(const char* haystack, const char* needle);
int LCpthread_setname_np(pthread_t thread, const char* name);
int LCpthread_getname_np(pthread_t thread, char* name, size_t size);

#endif

#endif
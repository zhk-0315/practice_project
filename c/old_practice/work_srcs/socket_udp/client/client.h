#ifndef CLIENT__CLIENT_H
#define CLIENT__CLIENT_H

#include <bits/pthreadtypes.h>
#include <pthread.h>

typedef struct CliFlag {
    unsigned char boolInitCliInfo : 1;
    pthread_mutex_t mutex;
} CliFlag;

CliFlag* GetCliFlag(void);

#endif
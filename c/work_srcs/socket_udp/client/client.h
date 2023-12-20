#ifndef CLIENT__CLIENT_H
#define CLIENT__CLIENT_H

#include <bits/pthreadtypes.h>
#include <pthread.h>

typedef struct Flag {
    unsigned char boolInitCliInfo : 1;
    pthread_mutex_t mutex;
} Flag;

Flag* GetFlagManager(void);

#endif
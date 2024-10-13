#ifndef CLIENT__CLIENT_LCMSGQUEUE__CLIENT_LCMSGQUEUE_H
#define CLIENT__CLIENT_LCMSGQUEUE__CLIENT_LCMSGQUEUE_H

#include "lcmsg_queue.h"
#include "list.h"
#include <pthread.h>

typedef struct CliMsgQueue {
    struct list_head* queue;
    pthread_mutex_t mutex;
} CliMsgQueue;

int DestoryCliMsgQueue(void);
int EnCliMsgQueue(const LcMsg* _msg);
int DeCliMsgQueue(LcMsg* _msg);

#endif
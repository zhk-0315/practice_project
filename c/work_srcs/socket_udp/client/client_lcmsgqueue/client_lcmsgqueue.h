#ifndef CLIENT__CLIENT_LCMSGQUEUE__CLIENT_LCMSGQUEUE_H
#define CLIENT__CLIENT_LCMSGQUEUE__CLIENT_LCMSGQUEUE_H

#include "lcmsg_queue.h"
#include "list.h"
#include <pthread.h>

typedef struct CliMsgQueueManager {
    struct list_head* msgQueue;
    pthread_mutex_t mutex;
} CliMsgQueueManager;

int DestoryCliMsgQueue(void);
CliMsgQueueManager* GetCliMsgQueueManager(void);
int CliEnMsgQueue(const LcMsg* _msg);
int CliDeMsgQueue(LcMsg* _msg);

#endif
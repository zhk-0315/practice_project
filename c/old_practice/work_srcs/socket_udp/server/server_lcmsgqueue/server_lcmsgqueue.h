#ifndef SERVER__SERVER_LCMSGQUEUE__SERVER_LCMSGQUEUE_H
#define SERVER__SERVER_LCMSGQUEUE__SERVER_LCMSGQUEUE_H

#include "lcmsg_queue.h"
#include "list.h"

typedef struct SrvMsgQueue {
    struct list_head* queue;
    pthread_mutex_t mutex;
} SrvMsgQueue;

int DestorySrvMsgQueue(void);
int EnSrvMsgQueue(const LcMsg* _lcMsg);
int DeSrvMsgQueue(LcMsg* _lcMsg);

#endif
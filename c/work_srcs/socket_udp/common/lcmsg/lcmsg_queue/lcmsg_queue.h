#ifndef COMMON__LCMSG__MSG_QUEUE_H
#define COMMON__LCMSG__MSG_QUEUE_H

#include "lcmsg.h"
#include "list.h"

typedef struct lcMsgQueueNode {
    LcMsg lcMsg;
    struct list_head list;
} lcMsgQueueNode;

#ifndef SOCK_UDP_QUEUERET
#define SOCK_UDP_QUEUERET
#define QUEUE_ERROR (-1)
enum {
    QUEUE_SUCCESS,
    QUEUE_EMPTY,
    QUEUE_FULL
};
#endif

struct list_head* InitMsgQueue(void);
int EnMsgQueue(struct list_head* msgQueue, const LcMsg* _lcMsg);
int DeMsgQueue(struct list_head* msgQueue, LcMsg* _lcMsg);
int DestoryMsgQueue(struct list_head** _msgQueue);

#endif
#ifndef COMMON__LCMSG__MSG_QUEUE_H
#define COMMON__LCMSG__MSG_QUEUE_H

#include "lcmsg.h"
#include "list.h"

typedef struct lcMsgListNode {
    LcMsg lcMsg;
    // list at the end
    struct list_head list;
} lcMsgListNode;

#ifndef QUEUE_ERROR
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
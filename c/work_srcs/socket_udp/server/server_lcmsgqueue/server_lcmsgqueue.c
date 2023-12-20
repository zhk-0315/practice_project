#include <stdio.h>
#include <stdlib.h>

#include "lcmsg_queue.h"

struct list_head* InitMsgQueue(void);
int EnMsgQueue(struct list_head* msgQueue, const LcMsg* _lcMsg);
int DeMsgQueue(struct list_head* msgQueue, LcMsg* _lcMsg);
int DestoryMsgQueue(struct list_head** _msgQueue);

static struct list_head* srvMsgQueue = NULL;
static int InitServerQueue(void)
{
    if (!srvMsgQueue) {
        srvMsgQueue = (struct list_head*)malloc(sizeof(struct list_head));
    }

    return 0;
}

int DestorySrvMsgQueue(void)
{
    if (!srvMsgQueue)
        return 0;

    DestoryMsgQueue(&srvMsgQueue);

    return 0;
}
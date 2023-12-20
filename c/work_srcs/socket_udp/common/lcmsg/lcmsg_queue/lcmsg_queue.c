#include <stdio.h>
#include <stdlib.h>

#include "lcmsg_queue.h"
#include "list.h"

struct list_head* InitMsgQueue(void)
{
    struct list_head* msgQueue = NULL;

    msgQueue = (struct list_head*)malloc(sizeof(struct list_head));
    if (msgQueue == NULL)
        return NULL;

    INIT_LIST_HEAD(msgQueue);

    return msgQueue;
}

int EnMsgQueue(struct list_head* msgQueue, const LcMsg* _lcMsg)
{
    lcMsgQueueNode* msgQueueNode = NULL;

    if (msgQueue == NULL)
        return QUEUE_ERROR;

    msgQueueNode = (lcMsgQueueNode*)malloc(sizeof(lcMsgQueueNode));
    if (_lcMsg != NULL)
        msgQueueNode->lcMsg = *_lcMsg;

    list_add_tail(&msgQueueNode->list, msgQueue);

    return QUEUE_SUCCESS;
}

int DeMsgQueue(struct list_head* msgQueue, LcMsg* _lcMsg)
{
    lcMsgQueueNode* msgQueueNode = NULL;

    if (msgQueue == NULL)
        return QUEUE_ERROR;

    if (list_empty(msgQueue))
        return QUEUE_EMPTY;

    msgQueueNode = list_entry(msgQueue->next, lcMsgQueueNode, list);
    if (_lcMsg != NULL)
        *_lcMsg = msgQueueNode->lcMsg;

    list_del(&msgQueueNode->list);
    free(msgQueueNode);

    return QUEUE_SUCCESS;
}

int DestoryMsgQueue(struct list_head** _msgQueue)
{
    struct list_head* msgQueue = *_msgQueue;

    if (msgQueue == NULL)
        return QUEUE_SUCCESS;

    while (DeMsgQueue(msgQueue, NULL) != QUEUE_EMPTY) { }

    free(msgQueue);
    *_msgQueue = NULL;

    return QUEUE_SUCCESS;
}
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbgout.h"
#include "link_queue.h"
#include "pre_process_queue.h"

LinkQueue* InitLinkQueue(size_t dataSize)
{
    LinkQueue* linkQueue = NULL;
    LinkNode* linkNode = NULL;

    if (dataSize <= 0) {
        QueueDbgout("param dataSize error");
        return NULL;
    }

    linkNode = (LinkNode*)malloc(sizeof(LinkNode));
    if (linkNode == NULL) {
        QueueDbgout("malloc linkNode error: %s", strerror(errno));
    }
    linkNode->data = NULL;
    linkNode->next = NULL;

    linkQueue = (LinkQueue*)malloc(sizeof(LinkQueue));
    if (linkQueue == NULL) {
        QueueDbgout("malloc linkQueue error: %s", strerror(errno));
    }
    linkQueue->front = linkNode;
    linkQueue->rear = linkNode;
    linkQueue->dataSize = dataSize;

    return linkQueue;
}

int EnLinkQueue(LinkQueue* linkQueue, const void* data, size_t dataSize)
{
    LinkNode* linkNode = NULL;

    if (linkQueue->dataSize != dataSize) {
        QueueDbgout("data size error!\n");
        return RET_ERROR;
    }

    linkNode = (LinkNode*)malloc(sizeof(LinkNode));
    if (linkNode == NULL) {
        QueueDbgout("malloc linkNode error: %s", strerror(errno));
        return RET_ERROR;
    }
    linkNode->data = NULL;
    linkNode->next = NULL;

    if (linkQueue->rear->data == NULL) {
        linkQueue->rear->data = malloc(linkQueue->dataSize);
        if (linkQueue->rear->data == NULL) {
            QueueDbgout("malloc data error: %s", strerror(errno));
            return RET_ERROR;
        }
    }

    if (data != NULL && linkQueue->front->data != NULL) {
        memcpy(linkQueue->rear->data, data, linkQueue->dataSize);
    }
    linkQueue->rear->next = linkNode;
    linkQueue->rear = linkNode;

    return QUEUE_SUCCESS;
}

int DeLinkQueue(LinkQueue* linkQueue, void* data, size_t dataSize)
{
    LinkNode* tmp = NULL;

    if (linkQueue->dataSize != dataSize) {
        QueueDbgout("data size error!\n");
        return RET_ERROR;
    }

    if (linkQueue->front == linkQueue->rear) {
        //QueueDbgout("link queue empty!\n");
        return QUEUE_EMPTY;
    }

    tmp = linkQueue->front->next;

    if (data != NULL && linkQueue->front->data != NULL) {
        memcpy(data, linkQueue->front->data, linkQueue->dataSize);
    }
    free(linkQueue->front->data);
    linkQueue->front->data = NULL;
    linkQueue->front->next = NULL;
    free(linkQueue->front);
    linkQueue->front = tmp;

    tmp = NULL;

    return QUEUE_SUCCESS;
}

int DestoryLinkQueue(LinkQueue** _linkQueue)
{
    LinkQueue* linkQueue = NULL;

    linkQueue = *_linkQueue;
    if (linkQueue == NULL) {
        QueueDbgout("param _linkQueue error");
        return RET_ERROR;
    }
    while (linkQueue->front != linkQueue->rear) {
        DeLinkQueue(linkQueue, NULL, linkQueue->dataSize);
    }
    free(linkQueue->front);
    free(linkQueue);

    linkQueue = NULL;
    *_linkQueue = NULL;

    return QUEUE_SUCCESS;
}
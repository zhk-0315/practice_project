#ifndef __LINK_LINK_QUEUE_C_
#define __LINK_LINK_QUEUE_C_

#include <stddef.h>

typedef struct LinkNode {
    void* data;
    struct LinkNode* next;
} LinkNode;

typedef struct LinkQueue {
    LinkNode* front;
    LinkNode* rear;
    size_t dataSize;
} LinkQueue;

LinkQueue* InitLinkQueue(size_t dataSize);
int EnLinkQueue(LinkQueue* linkQueue, const void* data, size_t dataSize);
int DeLinkQueue(LinkQueue* linkQueue, void* data, size_t dataSize);
int DestoryLinkQueue(LinkQueue** _linkQueue);

#endif
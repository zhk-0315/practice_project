#ifndef __SEQ_SEQ_QUEUE_H_
#define __SEQ_SEQ_QUEUE_H_

#include <stddef.h>

typedef struct SeqQueue {
    int front;
    int rear;
    size_t dataSize;
    int nodeCnt;
    void* queuePtr;

} SeqQueue;

SeqQueue* InitSeqQueue(int queueLen, size_t dataSize);
int EnSeqQueue(SeqQueue* seqQueue, const void* data, size_t dataSize);
int DeSeqQueue(SeqQueue* seqQueue, void* data, size_t dataSize);
int DestorySeqQueue(SeqQueue** _seqQueue);

#endif
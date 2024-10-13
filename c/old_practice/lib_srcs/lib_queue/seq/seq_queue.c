#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pre_process_queue.h"
#include "seq_queue.h"

SeqQueue* InitSeqQueue(int queueLen, size_t dataSize)
{
    SeqQueue* seqQueue = NULL;

    seqQueue = (SeqQueue*)malloc(sizeof(SeqQueue));

    seqQueue->dataSize = dataSize;
    seqQueue->nodeCnt = queueLen + 1;
    seqQueue->queuePtr = calloc(seqQueue->nodeCnt, seqQueue->dataSize);
    if (seqQueue->queuePtr == NULL) {
        QueueDbgout("calloc queuePtr error: %s", strerror(errno));
        return NULL;
    }
    seqQueue->front = 0;
    seqQueue->rear = 0;

    return seqQueue;
}

int EnSeqQueue(SeqQueue* seqQueue, const void* data, size_t dataSize)
{
    if (seqQueue->dataSize != dataSize) {
        QueueDbgout("dataSize error");
    }

    if ((seqQueue->rear + 1) % seqQueue->nodeCnt == seqQueue->front) {
        QueueDbgout("seqQueue full");
        return QUEUE_FULL;
    }

    memcpy(seqQueue->queuePtr + seqQueue->rear * seqQueue->dataSize,
        data, seqQueue->dataSize);
    seqQueue->rear = (seqQueue->rear + 1) % seqQueue->nodeCnt;

    return QUEUE_SUCCESS;
}

int DeSeqQueue(SeqQueue* seqQueue, void* data, size_t dataSize)
{
    if (seqQueue->dataSize != dataSize) {
        QueueDbgout("dataSize error");
    }

    if (seqQueue->front == seqQueue->rear) {
        QueueDbgout("seqQueue empty");
        return QUEUE_EMPTY;
    }

    memcpy(data, seqQueue->queuePtr + seqQueue->front * seqQueue->dataSize,
        seqQueue->dataSize);
    seqQueue->front = (seqQueue->front + 1) % seqQueue->nodeCnt;

    return QUEUE_SUCCESS;
}

int DestorySeqQueue(SeqQueue** _seqQueue)
{
    SeqQueue* seqQueue = NULL;

    seqQueue = *_seqQueue;
    free(seqQueue->queuePtr);
    free(seqQueue);

    seqQueue = NULL;
    *_seqQueue = NULL;

    return QUEUE_SUCCESS;
}

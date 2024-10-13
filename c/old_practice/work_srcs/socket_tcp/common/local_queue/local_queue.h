#ifndef __LOCAL_QUEUE_LOCAL_QUEUE_H_
#define __LOCAL_QUEUE_LOCAL_QUEUE_H_

#include "pre_process.h"

int EnMsgLinkQueue(const MsgBuf* msgBuf);
int DeMsgLinkQueue(MsgBuf* msgBuf);

#endif
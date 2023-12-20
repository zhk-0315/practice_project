#ifndef SERVER__SERVER_LCMSGQUEUE__SERVER_LCMSGQUEUE_H
#define SERVER__SERVER_LCMSGQUEUE__SERVER_LCMSGQUEUE_H

#include "lcmsg_queue.h"

int DestorySrvMsgQueue(void);
int EnSrvMsgQueue(const LcMsg* _lcMsg);
int DeSrvMsgQueue(LcMsg* _lcMsg);

#endif
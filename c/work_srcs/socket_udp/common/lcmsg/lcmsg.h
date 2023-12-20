#ifndef COMMON__LCMSG__LCMSG_H
#define COMMON__LCMSG__LCMSG_H

#include <sys/types.h>

typedef enum MsgType {
    UnknowMsgType,
    SaveCliInfo,
    DataMsg
} MsgType;

typedef pid_t EndID;
#define LCMSG_DATA_MAXLEN (256)
typedef union LcMsg {
    struct __LcMsg {
        MsgType msgType;
        EndID srcEndID;
        EndID destEndID;
        unsigned char data[LCMSG_DATA_MAXLEN];
    } msg;
    unsigned char tmp[sizeof(struct __LcMsg) + 1];
} LcMsg;

int InitTxBuf(LcMsg* TxBuf, EndID destEndID);
int InitRxBuf(LcMsg* RxBuf);
LcMsg* InitLcMsgBuf(int bufCnt);
int DestoryLcMsgBuf(LcMsg** _msgBuf);
int SetCheckSum(LcMsg* msgBuf);
int CheckCheckSum(LcMsg* msgBuf);
int ClearMsgBuf(LcMsg* msgBuf, MsgType msgType);

#endif
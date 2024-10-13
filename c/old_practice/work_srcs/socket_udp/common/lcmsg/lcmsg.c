#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lcmsg.h"
#include "lcmsg_queue.h"
#include "pre_process.h"

int ClearMsgBuf(LcMsg* msgBuf, MsgType msgType)
{
    msgBuf->msg.msgType = msgType;
    memset(msgBuf->msg.data, 0, LCMSG_DATA_MAXLEN);

    return 0;
}

int InitTxBuf(LcMsg* TxBuf, EndID destEndID)
{
    TxBuf->msg.msgType = DataMsg;
    TxBuf->msg.srcEndID = GetModulesManager()->LcEndID;
    TxBuf->msg.destEndID = destEndID;
    memset(TxBuf->msg.data, 0, LCMSG_DATA_MAXLEN);

    return 0;
}

int InitRxBuf(LcMsg* RxBuf)
{
    memset(RxBuf, 0, sizeof(LcMsg));

    return 0;
}

LcMsg* InitLcMsgBuf(int bufCnt)
{
    return (LcMsg*)calloc(bufCnt, sizeof(LcMsg));
}

int DestoryLcMsgBuf(LcMsg** _msgBuf)
{
    if (*_msgBuf == NULL)
        return 0;

    free(*_msgBuf);
    *_msgBuf = NULL;

    return 0;
}

void DisplayMsgBuf(LcMsg* msgBuf)
{
    int i = 0;
#define fp stdout
    // char path[128] = { 0 };
    // static FILE* fp = NULL;

    // if (!fp) {
    //     getcwd(path, sizeof(path));
    //     strcat(path, "/output");
    //     fp = fopen(path, "w+");
    //     LocalDbgout("fopen path %s", path);
    // }

    fprintf(fp, "msg from ID(%d) to ID(%d)", msgBuf->msg.srcEndID, msgBuf->msg.destEndID);

    for (i = 0; i < 50; i++) {
        fprintf(fp, "%u ", msgBuf->msg.data[i]);
    }
    fprintf(fp, "\n");
    fflush(fp);
}

int SetCheckSum(LcMsg* msgBuf)
{
    unsigned char sum = 0;
    int i = 0;

    for (i = 0; i < sizeof(msgBuf->msg); i++) {
        sum = (sum + msgBuf->tmp[i]) & 0xff;
    }

    msgBuf->tmp[i] = sum;

    return 0;
}

int CheckCheckSum(LcMsg* msgBuf)
{
    unsigned char sum = 0;
    int i = 0;

    for (i = 0; i < sizeof(msgBuf->msg); i++) {
        sum = (sum + msgBuf->tmp[i]) & 0xff;
    }

    if (sum == msgBuf->tmp[i]) {
        return 1;
    } else {
        return 0;
    }
}
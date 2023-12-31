#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include "client_threadpool.h"
#include "client_udp.h"
#include "lcmsg.h"
#include "pre_process.h"
#include "safe_api.h"
#include "thread_pool.h"

CliUdp* GetCliUdp(void)
{
    static CliUdp udp = {
        .addrlen = (socklen_t)sizeof(struct sockaddr_in),
    };

    return &udp;
}

int InitClientUdp(void)
{
    int sRet = 0;
    int udpFdTmp = 0;
    int optval = 1;
    struct sockaddr_in srvAddrTmp = { 0 };

    udpFdTmp = socket(AF_INET, SOCK_DGRAM, 0);
    if (CHECK_FUNCRET_ERR(udpFdTmp, -1, "socket udpFdTmp error"))
        return -1;

    sRet = setsockopt(udpFdTmp, SOL_SOCKET, SO_REUSEADDR, &optval, (socklen_t)sizeof(optval));
    if (CHECK_FUNCRET_ERR(sRet, -1, "setsockopt udpFdTmp SO_REUSEADDR error"))
        return -1;

    srvAddrTmp.sin_family = AF_INET;
    srvAddrTmp.sin_addr.s_addr = inet_addr(SERVER_IP);
    srvAddrTmp.sin_port = htons(SERVER_PORT);

    GetCliUdp()->udpFd = udpFdTmp;
    GetCliUdp()->srvAddr = srvAddrTmp;

    LocalDbgout("init CliUdp fd(%d)", udpFdTmp);

    return 0;
}

int DestoryCliUdp(void)
{
    if (!FD_CHECK(GetCliUdp()->udpFd))
        return -1;

    close(GetCliUdp()->udpFd);

    return 0;
}

static void* ProcessServerMsg(void* arg)
{
    LcMsg* msgBuf = (LcMsg*)arg;

    if (msgBuf->msg.msgType == SaveCliInfo) {
        pthread_mutex_lock(&GetCliFlag()->mutex);
        GetCliFlag()->boolInitCliInfo = 1;
        pthread_mutex_unlock(&GetCliFlag()->mutex);
    } else if (msgBuf->msg.msgType == DataMsg) {
        DisplayMsgBuf(msgBuf);
    }

    free(msgBuf);

    return NULL + 1;
}

static void* RecvServerMsgThread(void* arg)
{
    pthread_t selfTid = pthread_self();
    size_t RxSize = 0;
    LcMsg* RxBuf = (LcMsg*)malloc(sizeof(LcMsg));

    LCpthread_setname_np(selfTid, "RecvSrvMsg");
    pthread_detach(selfTid);

    while (1) {
        RxSize = recvfrom(GetCliUdp()->udpFd, RxBuf, sizeof(LcMsg), 0, NULL,
            (socklen_t*)&GetCliUdp()->addrlen);
        if (RxSize == -1) {
            LocalDbgout("recvfrom server error");
            continue;
        } else if (RxSize == 0) {
            continue;
        }

        AddTaskToClientPoolReleaseArgMem(ProcessServerMsg, RxBuf, sizeof(LcMsg));
    }

    free(RxBuf);

    return NULL;
}

int CreateRecvServerMsgThread(void)
{
    int s32Ret = 0;
    pthread_t tid = 0;

    s32Ret = pthread_create(&tid, NULL, RecvServerMsgThread, NULL);
    CHECK_FUNCRET_SUC(s32Ret, 0, "create RecvServerMsgThread error");

    LocalDbgout("CreateRecvServerMsgThread complete");

    return 0;
}

void* SendMsgBufToServer(void* arg)
{
    size_t TxSize = 0;
    LcMsg* TxBuf = (LcMsg*)arg;

    SetCheckSum(TxBuf);
    TxSize = sendto(GetCliUdp()->udpFd, TxBuf, sizeof(LcMsg), 0, (const struct sockaddr*)&GetCliUdp()->srvAddr,
        GetCliUdp()->addrlen);
    CHECK_FUNCRET_ERR(TxSize, -1, "send to server msg error");

    free(TxBuf);

    return NULL + 1;
}

int SendMsgToServer(void)
{
    size_t RxSize = 0;
    unsigned char* RxData = (unsigned char*)malloc(LCMSG_DATA_MAXLEN);
    LcMsg* TxBuf = (LcMsg*)malloc(sizeof(LcMsg));

    LCclear();
    while (1) {
        LCprintf("请输入消息内容: ");
        memset(RxData, 0, LCMSG_DATA_MAXLEN);
        RxSize = read(STDIN_FILENO, RxData, LCMSG_DATA_MAXLEN);
        if (RxSize == -1) {
            LocalDbgout("read form STDIN_FILENO error: %s", strerror(errno));
            continue;
        } else if (RxSize == 0) {
            continue;
        }

        if (LCstrcasestr((const char*)RxData, "zkquit"))
            break;

        InitTxBuf(TxBuf, SETVER_ENDID);
        memcpy(TxBuf->msg.data, RxData, LCMSG_DATA_MAXLEN);
        AddTaskToClientPoolReleaseArgMem(SendMsgBufToServer, TxBuf, sizeof(LcMsg));
    }
    LCclear();

    free(TxBuf);
    free(RxData);

    return 0;
}

int SendMsgToOtherClient(void)
{
    EndID remoteEndID = 0;
    size_t RxSize = 0;
    unsigned char* RxData = (unsigned char*)malloc(LCMSG_DATA_MAXLEN);
    LcMsg* TxBuf = (LcMsg*)malloc(sizeof(LcMsg));

    LCclear();
    LCprintf("请输入其他client的EndID: ");
    LCscanf("%d", &remoteEndID);

    while (1) {
        LCprintf("请输入消息内容: ");
        memset(RxData, 0, LCMSG_DATA_MAXLEN);
        RxSize = read(STDIN_FILENO, RxData, LCMSG_DATA_MAXLEN);
        if (RxSize == -1) {
            LocalDbgout("read form STDIN_FILENO error: %s", strerror(errno));
            continue;
        } else if (RxSize == 0) {
            continue;
        }

        if (LCstrcasestr((const char*)RxData, "zkquit"))
            break;

        InitTxBuf(TxBuf, remoteEndID);
        memcpy(TxBuf->msg.data, RxData, LCMSG_DATA_MAXLEN);
        AddTaskToClientPoolReleaseArgMem(SendMsgBufToServer, TxBuf, sizeof(LcMsg));
    }
    LCclear();

    free(TxBuf);
    free(RxData);

    return 0;
}

int InitClientInfoToServer(void)
{
    LcMsg* TxBuf = (LcMsg*)malloc(sizeof(LcMsg));

    TxBuf->msg.msgType = SaveCliInfo;
    TxBuf->msg.srcEndID = GetModulesManager()->LcEndID;
    TxBuf->msg.destEndID = SETVER_ENDID;

    AddTaskToClientPool(SendMsgBufToServer, TxBuf);

    return 0;
}

#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "clientinfo_list.h"
#include "lcmsg.h"
#include "pre_process.h"
#include "safe_api.h"
#include "server_lcmsgqueue.h"
#include "server_threadpool.h"
#include "server_udp.h"
#include "task_queue.h"

SrvUdp* GetSrvUdp(void)
{
    static SrvUdp udp = {
        .addrlen = sizeof(struct sockaddr),
    };

    return &udp;
}

int InitSrvUdp(void)
{
    int udpfd = -1;
    int sRet = 0;
    const int ov = 1;
    struct sockaddr_in LcAddr = { 0 };

    if (FD_CHECK(GetSrvUdp()->udpFd))
        return -1;

    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    CHECK_FUNCRET_ERR(udpfd, -1, "socket udpfd error");

    sRet = setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &ov, sizeof(ov));
    CHECK_FUNCRET_ERR(sRet, -1, "setsockopt udpfd SO_REUSEADDR error");

    LcAddr.sin_family = AF_INET;
    LcAddr.sin_addr.s_addr = INADDR_ANY;
    LcAddr.sin_port = htons(SERVER_PORT);
    sRet = bind(udpfd, (const struct sockaddr*)&LcAddr, GetSrvUdp()->addrlen);
    CHECK_FUNCRET_ERR(sRet, -1, "setsockopt udpfd SO_REUSEADDR error");

    GetSrvUdp()->udpFd = udpfd;
    GetSrvUdp()->LcAddr = LcAddr;

    LocalDbgout("init SrvUdp fd(%d)", udpfd);

    return 0;
}

int DestorySrvUdp(void)
{
    if (!FD_CHECK(GetSrvUdp()->udpFd))
        return -1;

    return close(GetSrvUdp()->udpFd);
}

void* ProcessStdin(void* arg)
{
    size_t RxSize = 0;
    int fd = 0;
    LcMsg MsgBuf = { 0 };

    fd = *(int*)arg;
    free(arg);

    InitTxBuf(&MsgBuf, SETVER_ENDID);
    RxSize = read(fd, MsgBuf.msg.data, LCMSG_DATA_MAXLEN);
    if (RxSize == -1) {
        LocalDbgout("read fd(%d) error!", fd);
        return NULL;
    } else if (RxSize == 0) {
        return NULL + 1;
    }

    return NULL + 1;
}

void* PorcessUdpMsg(void* arg)
{
    size_t RxSize = 0;
    size_t TxSize = 0;
    int fd = 0;
    LcMsg RxBuf = { 0 };
    LcMsg TxBuf = { 0 };
    struct sockaddr_in sockaddr = { 0 };

    fd = *(int*)arg;
    free(arg);

    RxSize = recvfrom(fd, &RxBuf, sizeof(LcMsg), 0, (struct sockaddr*)&sockaddr,
        (socklen_t*)&GetSrvUdp()->addrlen);
    if (RxSize == -1) {
        LocalDbgout("recvfrom fd(%d) error!", fd);
        return NULL;
    } else if (RxSize == 0) {
        return NULL + 1;
    }

    if (RxBuf.msg.msgType == SaveCliInfo
        && RxBuf.msg.destEndID == GetModulesManager()->LcEndID) {
        AddNodeToCliInfoList(RxBuf.msg.srcEndID, &sockaddr);

        TxBuf.msg.msgType = SaveCliInfo;
        TxBuf.msg.srcEndID = GetModulesManager()->LcEndID;
        TxBuf.msg.destEndID = RxBuf.msg.srcEndID;
        TxSize = sendto(GetSrvUdp()->udpFd, &TxBuf, sizeof(LcMsg), 0,
            (struct sockaddr*)&sockaddr, GetSrvUdp()->addrlen);
        CHECK_FUNCRET_SUC(TxSize, sizeof(LcMsg),
            "send to client init cliInfo error");
    } else if (RxBuf.msg.msgType == DataMsg) {
        EnSrvMsgQueue(&RxBuf);
    }

    return NULL + 1;
}

static void* SrvMsgProcessTask(void* arg)
{
    size_t TxSize = 0;
    LcMsg* msgBuf = (LcMsg*)arg;
    ClisInfoListNode destCliInfo = { 0 };

    if (!msgBuf)
        return NULL;

    if (msgBuf->msg.msgType == DataMsg) {
        if (msgBuf->msg.destEndID == SETVER_ENDID) {
            DisplayMsgBuf(msgBuf);
            return NULL + 1;
        }
        destCliInfo = *GetCliInfoNodeByCliID(msgBuf->msg.destEndID);
        TxSize = sendto(GetSrvUdp()->udpFd, msgBuf, sizeof(LcMsg), 0,
            (struct sockaddr*)&destCliInfo.addr, GetSrvUdp()->addrlen);
        if (TxSize != sizeof(LcMsg)) {
            LocalDbgout("send msg from Endid(%d) to Endid(%d) faild",
                msgBuf->msg.srcEndID, msgBuf->msg.destEndID);
        }
    }

    free(msgBuf);

    return NULL + 1;
}

static void* SrvMsgProcessThread(void* arg)
{
    pthread_t tid = pthread_self();
    LcMsg* msgBuf = malloc(sizeof(LcMsg));

    pthread_detach(tid);
    LCpthread_setname_np(tid, "SrvMsgProcess");

    while (1) {
        if (DeSrvMsgQueue(msgBuf) == QUEUE_EMPTY) {
            usleep(50000);
            continue;
        }

        AddTaskToServerPoolReleaseArgMem(SrvMsgProcessTask, (void*)msgBuf,
            sizeof(LcMsg));
    }

    free(msgBuf);

    return arg;
}

int CreateSrvMsgProcessThread(void)
{
    int sRet = 0;
    pthread_t tid = 0;

    sRet = pthread_create(&tid, NULL, SrvMsgProcessThread, NULL);
    CHECK_FUNCRET_SUC(sRet, 0, "create ServerEpollThread error");

    LocalDbgout("CreateSrvMsgProcessThread complete");

    return 0;
}
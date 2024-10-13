#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client_info.h"
#include "local_queue.h"
#include "pre_process.h"
#include "pre_process_queue.h"
#include "server_dbgout.h"
#include "server_epoll.h"
#include "server_socket.h"

static int serverFd = -1;
static socklen_t addrlen = sizeof(struct sockaddr);

int GetServerFd(void)
{
    return serverFd;
}

socklen_t GetAddrlen(void)
{
    return addrlen;
}

int InitServerSock(void)
{
    int s32Ret = 0;
    const int optval = 1;
    struct sockaddr_in localAddr = { 0 };

    if (serverFd != -1) {
        return -1;
    }
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        LocalDbgout("socket serverFd error: %s", strerror(errno));
        return -1;
    }

    s32Ret = setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR,
        &optval, sizeof(optval));
    if (s32Ret == -1) {
        LocalDbgout("setsockopt SO_REUSEADDR error: %s", strerror(errno));
        return -1;
    }

    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(SERVER_PORT);
    localAddr.sin_addr.s_addr = INADDR_ANY;
    s32Ret = bind(serverFd, (struct sockaddr*)&localAddr, addrlen);
    if (s32Ret == -1) {
        LocalDbgout("bind serverFd with localAddr error: %s", strerror(errno));
        return -1;
    }

    s32Ret = listen(serverFd, 3);
    if (s32Ret == -1) {
        LocalDbgout("listen serverFd error: %s", strerror(errno));
        return -1;
    }

    DebugDbgout("serverFd = %d", serverFd);

    return 0;
}

int AcceptNewClient(void)
{
    CliInfoNode cliInfoNode = { 0 };
    cliInfoNode.fd = accept(serverFd, (struct sockaddr*)&cliInfoNode.addr,
        (socklen_t*)&addrlen);
    if (cliInfoNode.fd == -1) {
        LocalDbgout("accept new cli fd error: %s", strerror(errno));
        return -1;
    }

    AddToCliInfo(&cliInfoNode);
    AddEpollByFd("new link", cliInfoNode.fd, EPOLLIN | EPOLLET);

    return 0;
}

int BroadcastAllClient(int msgFromFd)
{
    ssize_t sizeRx = 0;

    memset(GetMsgBuf(RX_BUF)->msg, 0, MAX_MSG_LEN);
    sizeRx = read(msgFromFd, GetMsgBuf(RX_BUF)->msg, MAX_MSG_LEN);
    if (sizeRx == -1) {
        LocalDbgout("read msgFromFd error: %s", strerror(errno));
        return -1;
    }

    DebugDbgout("BroadcastAllClient");

    return SendMsgAllClient(GetMsgBuf(RX_BUF));
}

int ClientMsgEnQueue(int fd)
{
    ssize_t sizeRx = 0;

    sizeRx = recv(fd, GetMsgBuf(RX_BUF), sizeof(MsgBuf), 0);
    if (sizeRx == -1) {
        LocalDbgout("recv cli msg error: %s", strerror(errno));
        return -1;
    } else if (sizeRx == 0) {
        LocalDbgout("shutdown link fd(%d)", fd);
        DelFromCliInfoByFd(fd);
        return -1;
    }

    if (GetMsgBuf(RX_BUF)->msgType == SAVE_CLI_INFO) {
        SaveOneCliInfoByFd(fd, GetMsgBuf(RX_BUF));
        SendInitInfoToCliByFd(fd);
    } else {
        EnMsgLinkQueue(GetMsgBuf(RX_BUF));
    }

    return 0;
}

#ifdef LOCAL_DEBUG
int DisplayMsgBuf(MsgBuf* msgBuf)
{
    char chstr[1024] = { 0 };
    char chtmp[64] = { 0 };
    int i = 0;
    EndID tmpid = 0;
    CliInfoNode* srcEndInfo = NULL;
    CliInfoNode* destEndInfo = NULL;

    srcEndInfo = GetOneCliInfoByEndID(msgBuf->srcEndID);

    if (msgBuf->destEndID == GetInitModules()->GetEndId()) {
        tmpid = msgBuf->destEndID;
    } else {
        destEndInfo = GetOneCliInfoByEndID(msgBuf->destEndID);
        tmpid = destEndInfo->cliEndid;
    }

    sprintf(chstr, "msg from id(%d) to id(%d)--:", srcEndInfo->cliEndid, tmpid);
    for (i = 0; i < 30; i++) {
        sprintf(chtmp, "%x ", msgBuf->msg[i]);
        strcat(chstr, chtmp);
    }

    strcat(chstr, "\n");
    DebugDbgout("%s", chstr);

    return 0;
}
#endif

int ProcessMsg(MsgBuf* msgBuf)
{
    ssize_t sizeTx = 0;
    CliInfoNode* srcEndInfo = NULL;
    CliInfoNode* destEndInfo = NULL;

    if (msgBuf->msgType != PROCESS_MSG) {
        LocalDbgout("msgType error");
        return -1;
    }

#ifdef LOCAL_DEBUG
    DisplayMsgBuf(msgBuf);
#endif

    srcEndInfo = GetOneCliInfoByEndID(msgBuf->srcEndID);
    if (msgBuf->destEndID == GetInitModules()->GetEndId()) {
        DebugDbgout("write");
        sizeTx = write(msgBuf->destEndID, msgBuf->msg, MAX_MSG_LEN);
        if (sizeTx != MAX_MSG_LEN) {
            LocalDbgout("send msg from %s to %d error!!!: %s",
                GetCliInfoErrStr(srcEndInfo), msgBuf->destEndID,
                strerror(errno));
            return -1;
        }

        return 0;
    }

    destEndInfo = GetOneCliInfoByEndID(msgBuf->destEndID);
    DebugDbgout("send");
    sizeTx = send(destEndInfo->fd, msgBuf, sizeof(MsgBuf), 0);
    if (sizeTx != sizeof(MsgBuf)) {
        LocalDbgout("send msg from %s to %s error!!!: %s",
            GetCliInfoErrStr(srcEndInfo), GetCliInfoErrStr(destEndInfo),
            strerror(errno));
        return -1;
    }

    return 0;
}

int ProcessCliMsg(void)
{
    MsgBuf msgBufTmp_ = { 0 };

    prctl(PR_SET_NAME, "SrvProMsg");

    while (1) {
        if (QUEUE_EMPTY == DeMsgLinkQueue(&msgBufTmp_)) {
            // DebugDbgout("Queue QUEUE_EMPTY");
            usleep(50);
            continue;
        }
        ProcessMsg(&msgBufTmp_);
    }

    return 0;
}

int CreateProcessCliMsgThread(void)
{
    int s32Ret = -1;
    pthread_t processMsgTid = 0;
    pthread_attr_t processMsgTAttr = { 0 };

    s32Ret = pthread_attr_setdetachstate(&processMsgTAttr,
        PTHREAD_CREATE_DETACHED);
    if (s32Ret != 0) {
        LocalDbgout("pthread_attr_setdetachstate EpollProcessThread error: %s",
            strerror(errno));
        return -1;
    }

    s32Ret = pthread_create(&processMsgTid, &processMsgTAttr,
        (void* (*)(void*))ProcessCliMsg, NULL);
    if (s32Ret != 0) {
        LocalDbgout("pthread_create EpollProcessThread error: %s",
            strerror(errno));
        return -1;
    }

    return 0;
}

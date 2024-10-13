#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include "client_dbgout.h"
#include "client_socket.h"
#include "compile_safe_api.h"
#include "local_queue.h"
#include "pre_process.h"
#include "pre_process_queue.h"

static int clientFd = -1;
const socklen_t addrlen = sizeof(struct sockaddr);

int GetClientFd(void)
{
    return clientFd;
}

socklen_t GetAddrLen(void)
{
    return addrlen;
}

int InitClientSock(void)
{
    int s32Ret = 0;
    struct sockaddr_in serverAddr = { 0 };

    if (clientFd != -1) {
        return -1;
    }
    clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd < 0) {
        LocalDbgout("socket clientFd error: %s", strerror(errno));
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP_STR);
    serverAddr.sin_port = htons(SERVER_PORT);
    s32Ret = connect(GetClientFd(), (struct sockaddr*)&serverAddr, addrlen);
    if (s32Ret == -1) {
        LocalDbgout("connect clientFd error: %s", strerror(errno));
        return s32Ret;
    }

    DebugDbgout("clientFd = %d", clientFd);

    return 0;
}

int TxToServer(void)
{
    ssize_t sizeRx = 0;
    ssize_t sizeTx = 0;

    while (1) {
        LCPRINTF("请输入要发送给server的消息: ");
        memset(GetMsgBuf(RX_BUF)->msg, 0, MAX_MSG_LEN);
        sizeRx = read(STDIN_FILENO, GetMsgBuf(RX_BUF)->msg, MAX_MSG_LEN);
        if (sizeRx < 0) {
            LocalDbgout("read STDIN_FILENO error: %s", strerror(errno));
            continue;
        }
        if (NULL != SafeStrcasestr((char*)GetMsgBuf(RX_BUF)->msg, "!!quit")) {
            break;
        }

        GetMsgBuf(TX_BUF)->msgType = PROCESS_MSG;
        GetMsgBuf(TX_BUF)->srcEndID = GetInitModules()->GetEndId();
        GetMsgBuf(TX_BUF)->destEndID = SERVER_ENDID;
        memcpy(GetMsgBuf(TX_BUF)->msg, GetMsgBuf(RX_BUF)->msg, MAX_MSG_LEN);
        sizeTx = send(GetClientFd(), GetMsgBuf(TX_BUF), sizeof(MsgBuf), 0);
        if (sizeTx != sizeof(MsgBuf)) {
            LocalDbgout("send clientFd error: %s", strerror(errno));
            continue;
        }
    }

    return 0;
}

int TxToOtherCli(void)
{
    ssize_t sizeRx = 0;
    ssize_t sizeTx = 0;
    EndID endIdTmp = 0;

    LCPRINTF("请输入要发送给其他cli的endid: ");
    LCSCANF("%d", &endIdTmp);

    while (1) {
        LCPRINTF("请输入要发送给其他cli的消息: ");
        memset(GetMsgBuf(RX_BUF)->msg, 0, MAX_MSG_LEN);
        sizeRx = read(STDIN_FILENO, GetMsgBuf(RX_BUF)->msg, MAX_MSG_LEN);
        if (sizeRx < 0) {
            LocalDbgout("read STDIN_FILENO error: %s", strerror(errno));
            continue;
        }
        if (NULL != SafeStrcasestr((char*)GetMsgBuf(RX_BUF)->msg, "!!quit")) {
            break;
        }

        GetMsgBuf(TX_BUF)->msgType = PROCESS_MSG;
        GetMsgBuf(TX_BUF)->srcEndID = GetInitModules()->GetEndId();
        GetMsgBuf(TX_BUF)->destEndID = endIdTmp;
        memcpy(GetMsgBuf(TX_BUF)->msg, GetMsgBuf(RX_BUF)->msg, MAX_MSG_LEN);
        sizeTx = send(GetClientFd(), GetMsgBuf(TX_BUF), sizeof(MsgBuf), 0);
        if (sizeTx != sizeof(MsgBuf)) {
            LocalDbgout("send clientFd error: %s", strerror(errno));
            continue;
        }
    }

    return 0;
}

int RxFromServer(void)
{
    ssize_t sizeRx = 0;

    memset(GetMsgBuf(RX_BUF)->msg, 0, MAX_MSG_LEN);
    sizeRx = recv(GetClientFd(), GetMsgBuf(RX_BUF), sizeof(MsgBuf), 0);
    if (sizeRx < 0) {
        LocalDbgout("recv clientFd error: %s", strerror(errno));
        return -1;
    } else if (sizeRx == 0) {
        DisplayStr("!!!NOTE: server shutdown link");
        sleep(3);
        // pthread_exit(NULL);
        return -1;
    }

    DebugDbgout("recv msg");

    if (GetMsgBuf(RX_BUF)->msgType == SAVE_CLI_INFO) {
        GetFlagManager()->initCliInfo = 1;
        DebugDbgout("initCliInfo");
        return 0;
    }

    DebugDbgout("msg enqueue");
    if (EnMsgLinkQueue(GetMsgBuf(RX_BUF)) == QUEUE_FULL) {
        return -1;
    } else {
        DebugDbgout("EnMsgLinkQueue");
        return 0;
    }
}

int RecvServerMsg(void)
{

    prctl(PR_SET_NAME, "CliRecv");

    InitCliInfoToServer();

    while (1) {
        RxFromServer();
    }

    return 0;
}

int CreateConnectServerThread(void)
{
    int s32Ret = -1;

    pthread_t srvMsgTid = 0;
    pthread_attr_t srvMsgThreadAttr = { 0 };

    s32Ret = pthread_attr_setdetachstate(&srvMsgThreadAttr,
        PTHREAD_CREATE_DETACHED);
    if (s32Ret != 0) {
        LocalDbgout("pthread_attr_setdetachstate ConnectServerThread error: %s",
            strerror(errno));
        return -1;
    }

    s32Ret = pthread_create(&srvMsgTid, &srvMsgThreadAttr,
        (void* (*)(void*))RecvServerMsg, NULL);
    if (s32Ret != 0) {
        LocalDbgout("pthread_create ConnectServerThread error: %s",
            strerror(errno));
        return -1;
    }

    DebugDbgout("CreateConnectServerThread sucess");

    return 0;
}

int DisplayMsgWithPid(MsgBuf* msgBuf)
{
    int i = 0;

    LCPRINTF("msg from %d(endid) to %d(endid): ", msgBuf->srcEndID,
        msgBuf->destEndID);
    for (i = 0; i < 30; i++) {
        LCPRINTF("%x ", msgBuf->msg[i]);
    }
    LCPRINTF("\n");

    return 0;
}

int ProcessFromMsgQueue(void)
{
    MsgBuf _msgBuf = { 0 };

    if (DeMsgLinkQueue(&_msgBuf) == QUEUE_EMPTY) {
        usleep(5000);
        return -1;
    }

    return DisplayMsgWithPid(&_msgBuf);
}

int ProcessQueueMsg(void)
{
    prctl(PR_SET_NAME, "CliProMsg");

    while (1) {
        ProcessFromMsgQueue();
    }

    return -1;
}

int CreateMsgQueueProcessThread(void)
{
    int s32Ret = -1;

    pthread_t msgProcessTid = 0;
    pthread_attr_t msgProcessThreadAttr = { 0 };

    s32Ret = pthread_attr_setdetachstate(&msgProcessThreadAttr,
        PTHREAD_CREATE_DETACHED);
    if (s32Ret != 0) {
        LocalDbgout("pthread_attr_setdetachstate MsgQueueProcessThread error: %s",
            strerror(errno));
        return -1;
    }

    s32Ret = pthread_create(&msgProcessTid, &msgProcessThreadAttr,
        (void* (*)(void*))ProcessQueueMsg, NULL);
    if (s32Ret != 0) {
        LocalDbgout("pthread_create MsgQueueProcessThread error: %s",
            strerror(errno));
        return -1;
    }

    DebugDbgout("CreateMsgQueueProcessThread sucess");

    return 0;
}

int InitCliInfoToServer(void)
{
    ssize_t sizeTx = 0;

    GetMsgBuf(TX_BUF)->msgType = SAVE_CLI_INFO;
    GetMsgBuf(TX_BUF)->srcEndID = GetInitModules()->GetEndId();
    GetMsgBuf(TX_BUF)->destEndID = SERVER_ENDID;
    sizeTx = send(GetClientFd(), GetMsgBuf(TX_BUF), sizeof(MsgBuf), 0);
    if (sizeTx != sizeof(MsgBuf)) {
        LocalDbgout("send clientFd error: %s", strerror(errno));
        return -1;
    }

    DebugDbgout("send InitCliInfoToServer success");

    return 0;
}

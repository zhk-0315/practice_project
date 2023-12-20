#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "client_info.h"
#include "list.h"
#include "pre_process.h"
#include "server_dbgout.h"
#include "server_socket.h"

static struct list_head* cliInfoManager = NULL;

struct list_head* GetCliInfoManager(void)
{
    if (cliInfoManager == NULL) {
        InitCliInfoManager();
    }

    return cliInfoManager;
}

int AddToCliInfo(CliInfoNode* cliInfo)
{
    CliInfoNode* cliInfoNode = NULL;

    if (cliInfo == NULL) {
        return -1;
    }

    DebugDbgout("add new cli fd = %d", cliInfo->fd);

    cliInfoNode = (CliInfoNode*)malloc(sizeof(CliInfoNode));
    memcpy(cliInfoNode, cliInfo, sizeof(CliInfoNode));
    cliInfoNode->cliEndid = 0;
    list_add(&cliInfoNode->list, GetCliInfoManager());

    return 0;
}

int DelFromCliInfoByFd(int fd)
{
    struct list_head* pos = NULL;
    CliInfoNode* posCliInfo = NULL;

    list_for_each(pos, GetCliInfoManager())
    {
        posCliInfo = list_entry(pos, CliInfoNode, list);
        if (posCliInfo->fd != fd)
            continue;
        list_del(pos);
        free(posCliInfo);
        CloseFd("clifd", fd);
        return 0;
    }

    return -1;
}

CliInfoNode* GetOneCliInfoByFd(int fd)
{
    struct list_head* pos = NULL;
    CliInfoNode* posCliInfo = NULL;

    list_for_each(pos, GetCliInfoManager())
    {
        posCliInfo = list_entry(pos, CliInfoNode, list);
        if (posCliInfo->fd != fd)
            continue;
        return posCliInfo;
    }

    return NULL;
}

CliInfoNode* GetOneCliInfoByEndID(EndID cliEndid)
{
    struct list_head* pos = NULL;
    CliInfoNode* posCliInfo = NULL;

    list_for_each(pos, GetCliInfoManager())
    {
        posCliInfo = list_entry(pos, CliInfoNode, list);
        if (posCliInfo->cliEndid != cliEndid)
            continue;
        DebugDbgout("GetOneCliInfoByEndID sucess id(%d)", posCliInfo->cliEndid);
        return posCliInfo;
    }

    DebugDbgout("GetOneCliInfoByEndID faild");
    return NULL;
}

int SaveOneCliInfoByFd(int fd, MsgBuf* msgBuf)
{
    struct list_head* pos = NULL;
    CliInfoNode* posCliInfo = NULL;

    list_for_each(pos, GetCliInfoManager())
    {
        posCliInfo = list_entry(pos, CliInfoNode, list);
        if (posCliInfo->fd != fd)
            continue;

        posCliInfo->cliEndid = msgBuf->srcEndID;
        DebugDbgout("SaveOneCliInfoByFd endid(%d) bsucess",
            posCliInfo->cliEndid);
        return 0;
    }

    return -1;
}

int InitCliInfoManager(void)
{
    if (cliInfoManager != NULL) {
        return -1;
    }

    cliInfoManager = (struct list_head*)malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(cliInfoManager);

    return 0;
}

const char* GetCliInfoErrStr(CliInfoNode* cliInfo)
{
    static char* errStr = NULL;

    if (errStr == NULL) {
        errStr = (char*)malloc(64);
    }

    sprintf(errStr, "%d(endid)", cliInfo->cliEndid);

    return errStr;
}

int SendMsgAllClient(const MsgBuf* msgBuf)
{
    int s32Ret = 0;
    ssize_t sizeTx = 0;
    struct list_head* pos = NULL;
    CliInfoNode* posCliInfo = NULL;

    list_for_each(pos, GetCliInfoManager())
    {
        posCliInfo = list_entry(pos, CliInfoNode, list);

        GetMsgBuf(TX_BUF)->msgType = PROCESS_MSG;
        GetMsgBuf(TX_BUF)->srcEndID = SERVER_ENDID;
        GetMsgBuf(TX_BUF)->destEndID = posCliInfo->cliEndid;
        memcpy(GetMsgBuf(TX_BUF)->msg, msgBuf->msg, MAX_MSG_LEN);
        sizeTx = send(posCliInfo->fd, GetMsgBuf(TX_BUF), sizeof(MsgBuf), 0);
        if (sizeTx != sizeof(MsgBuf)) {
            LocalDbgout("send client:%s error: %s", GetCliInfoErrStr(posCliInfo),
                strerror(errno));
            s32Ret = -1;
        }
    }

    DebugDbgout("SendMsgAllClient");

    return s32Ret;
}

int SendInitInfoToCliByFd(int fd)
{
    int s32Ret = 0;
    ssize_t sizeTx = 0;
    struct list_head* pos = NULL;
    CliInfoNode* posCliInfo = NULL;

    list_for_each(pos, GetCliInfoManager())
    {
        posCliInfo = list_entry(pos, CliInfoNode, list);
        if (posCliInfo->fd != fd)
            continue;

        GetMsgBuf(TX_BUF)->msgType = SAVE_CLI_INFO;
        GetMsgBuf(TX_BUF)->srcEndID = SERVER_ENDID;
        GetMsgBuf(TX_BUF)->destEndID = posCliInfo->cliEndid;
        sizeTx = send(fd, GetMsgBuf(TX_BUF), sizeof(MsgBuf), 0);
        if (sizeTx != sizeof(MsgBuf)) {
            LocalDbgout("send client:%s error: %s", GetCliInfoErrStr(posCliInfo),
                strerror(errno));
            s32Ret = -1;
        }
        return s32Ret;
    }

    return -1;
}
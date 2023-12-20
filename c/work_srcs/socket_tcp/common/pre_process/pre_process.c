#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pre_process.h"

InitModules* GetInitModules(void)
{
    static InitModules initAllModule = { 0 };

    return &initAllModule;
}

MsgBuf* GetMsgBuf(MsgBufType msgBufType)
{
    static MsgBuf* msgBuf = NULL;

    if (msgBuf == NULL) {
        msgBuf = (MsgBuf*)calloc(2, sizeof(MsgBuf));
        if (msgBuf == NULL) {
            LocalDbgout("calloc msgBuf error: %s", strerror(errno));
            return NULL;
        }
    }

    return (msgBuf + msgBufType);
}

int CloseFd(const char* info, int fd)
{
    int s32Ret = 0;

    s32Ret = close(fd);
    if (s32Ret == -1) {
        LocalDbgout("close %s error: %s", info, strerror(errno));
    }

    return s32Ret;
}

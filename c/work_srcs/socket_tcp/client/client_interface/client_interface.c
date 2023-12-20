#include <curses.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "client_interface.h"
#include "client_socket.h"
#include "dbgout.h"
#include "pre_process.h"

int CliInterface(void)
{
    int sw = 0;
    int (*Handeler[])(void) = {
        [0] = TxToServer,
        [1] = TxToOtherCli
    };

    prctl(PR_SET_NAME, "CliUI");

    while (1) {
        LCPRINTF("client endid = %d\n", GetInitModules()->GetEndId());
        DisplayStr("0. 发送消息到server");
        DisplayStr("1. 发送消息到其他client(pid)");
        DisplayStr("请输入选项: ");
        LCSCANF("%d", &sw);
        DebugDbgout("sw=%d", sw);

        if (sw >= sizeof(Handeler) / sizeof(int (*)(void))) {
            DisplayStr("输入错误，请查看说明");
            DisplayStr("输入错误，请查看说明");
            DisplayStr("输入错误，请查看说明");
            sleep(1);
        } else {
            Handeler[sw]();
        }

        clear();
    }

    return 0;
}

int CreateCliInterfaceThread(void)
{
    int s32Ret = -1;

    pthread_t cliInterfaceTid = 0;
    pthread_attr_t cliInterfaceThreadAttr = { 0 };

    s32Ret = pthread_attr_setdetachstate(&cliInterfaceThreadAttr,
        PTHREAD_CREATE_DETACHED);
    if (s32Ret != 0) {
        LocalDbgout("pthread_attr_setdetachstate CliInterfaceThread error: %s",
            strerror(errno));
        return -1;
    }

    s32Ret = pthread_create(&cliInterfaceTid, &cliInterfaceThreadAttr,
        (void* (*)(void*))CliInterface, NULL);
    if (s32Ret != 0) {
        LocalDbgout("pthread_create CliInterfaceThread error: %s",
            strerror(errno));
        return -1;
    }

    DebugDbgout("CreateCliInterfaceThread sucess");

    return 0;
}
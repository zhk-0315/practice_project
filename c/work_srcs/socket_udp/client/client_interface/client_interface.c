#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "client.h"
#include "client_interface.h"
#include "client_udp.h"
#include "pre_process.h"
#include "safe_api.h"

static int (*InterfaceLevel1[])(void) = {
    SendMsgToServer,
    SendMsgToOtherClient
};

static void* ClientInterface(void* arg)
{
    int sw = 0;
    int Index = 0;
    bool boolTmp = false;
    pthread_t selfTid = pthread_self();

    LCpthread_setname_np(selfTid, "CliUI");
    pthread_detach(selfTid);

#if 1
    while (Index++ < 5) {
        pthread_mutex_lock(&GetFlagManager()->mutex);
        boolTmp = GetFlagManager()->boolInitCliInfo;
        pthread_mutex_unlock(&GetFlagManager()->mutex);
        if (!boolTmp) {
            DisplayStr("being InitClientInfoToServer");
            InitClientInfoToServer();
        }
        usleep(500000);
    }

    while (1) {
        pthread_mutex_lock(&GetFlagManager()->mutex);
        boolTmp = GetFlagManager()->boolInitCliInfo;
        pthread_mutex_unlock(&GetFlagManager()->mutex);
        if (!boolTmp) {
            DisplayStr("InitClientInfoToServer error");
            sleep(1);
        } else {
            break;
        }
    }
#else
    InitClientInfoToServer();
    usleep(500000);
#endif

    LCclear();
    while (1) {
        Index = 0;
        DisplayStr("%d.发送消息给server", Index++);
        DisplayStr("%d.发送消息给其他client", Index++);
        // DisplayStr("%d.返回", swIndex++);
        LCprintf("请输入选项: ");
        LCscanf("%d", &sw);

        if (sw >= Index) {
            DisplayStr("!!!sw error->usage:sw < %d", Index);
            DisplayStr("!!!sw error->usage:sw < %d", Index);
            DisplayStr("!!!sw error->usage:sw < %d", Index);
            sleep(1);
            continue;
        }

        InterfaceLevel1[sw]();
    }

    return NULL;
}

int CreateClientInterfaceThread(void)
{
    int s32Ret = 0;
    pthread_t tid = 0;

    s32Ret = pthread_create(&tid, NULL, ClientInterface, NULL);
    CHECK_FUNCRET_SUC(s32Ret, 0, "create RecvServerMsgThread error");

    return 0;
}

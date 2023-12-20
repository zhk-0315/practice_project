#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "client_dbgout.h"
#include "client_interface.h"
#include "client_socket.h"
#include "pre_process.h"

FlagManager* GetFlagManager(void)
{
    static FlagManager _flag = { 0 };

    return &_flag;
}

EndID GetClientEndId(void)
{
    static EndID endId = 0;
    static void* init = NULL;

    if (init == NULL) {
        endId = getpid();
        init = (void*)1;
    }

    return endId;
}

int InitPreModules(void)
{
    GetInitModules()->GetLogFileInfo = GetTcpClientLogFileInfo;
    GetInitModules()->GetEndId = GetClientEndId;

    return 0;
}

void SignalHandler(int signal)
{
    switch (signal) {
    case SIGPIPE:
        break;
    default:
        break;
    }

    return;
}

int InitSigleHandle(void)
{
    signal(SIGPIPE, SignalHandler);

    return 0;
}

int PreInit(void)
{
    InitSigleHandle();
    InitPreModules();
    InitLogFile();

    return 0;
}

int main(int argc, char const* argv[])
{
    PreInit();
    LCPRINTF("start client endid(%d)\n", GetInitModules()->GetEndId());

    InitClientSock();

    CreateConnectServerThread();
    CreateMsgQueueProcessThread();
    CreateCliInterfaceThread();

    while (1) {
        usleep(500);
    }

    CloseFd("clientFd", GetClientFd());

    return 0;
}

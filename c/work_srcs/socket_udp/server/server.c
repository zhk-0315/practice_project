#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "pre_process.h"
#include "server_dbgout.h"
#include "server_epoll.h"
#include "server_threadpool.h"
#include "server_udp.h"

static int InitModulesManager(void)
{
    ToSetModulesManager()->GetLogFile = GetServerLogFile;
    ToSetModulesManager()->LcEndID = SETVER_ENDID;

    return 0;
}

static void SigHandler(int signo)
{
    switch (signo) {
    case SIGPIPE:
        // do nothing
        break;
    default:
        break;
    }
}

static int InitSignalsConfig(void)
{
    struct sigaction act_ = { 0 };

    // sigpipe
    act_.sa_flags = 0;
    act_.sa_handler = SigHandler;
    sigemptyset(&act_.sa_mask);
    sigaction(SIGPIPE, &act_, NULL);

    return 0;
}

static int ConfigPreInit(void)
{
    InitModulesManager();
    InitLogFile();
    InitSignalsConfig();
    usleep(10000);

    return 0;
}

static int InitModules(void)
{
    InitSrvUdp();
    InitServerEpoll();

    CreateServerThreadPool();
    CreateServerEpollThread();
    CreateSrvMsgProcessThread();

    return 0;
}

static int RecycleResource(void)
{
    DestoryServerEpoll();
    DestorySrvUdp();

    DestoryServerThreadPool();

    return 0;
}

int main(int argc, char const* argv[])
{
    ConfigPreInit();
    LocalDbgout("start server");
    InitModules();

    while (1) {
        sleep(5);
    }

    RecycleResource();

    return 0;
}

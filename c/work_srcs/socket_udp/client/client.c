#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "client_dbgout.h"
#include "client_interface.h"
#include "client_threadpool.h"
#include "client_udp.h"
#include "dbgout.h"
#include "pre_process.h"
#include "safe_api.h"

static Flag* g_flagManager = NULL;

static int InitFlagManager(void)
{
    if (g_flagManager == NULL) {
        g_flagManager = (Flag*)malloc(sizeof(Flag));
        pthread_mutex_init(&g_flagManager->mutex, NULL);
        memset(g_flagManager, 0, sizeof(Flag));
    }

    return 0;
}
static int DestoryFlagManager(void)
{
    if (g_flagManager != NULL) {
        pthread_mutex_destroy(&g_flagManager->mutex);
        free(g_flagManager);
        g_flagManager = NULL;
    }

    return 0;
}
Flag* GetFlagManager(void)
{
    if (g_flagManager == NULL)
        InitFlagManager();

    return g_flagManager;
}

static int InitModulesManager(void)
{
    ToSetModulesManager()->GetLogFile = GetClientLogFile;
    ToSetModulesManager()->LcEndID = getpid();

    return 0;
}

static void SigHandler(int signo)
{
    switch (signo) {
    case SIGPIPE:
    case SIGSEGV:
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

static int InitModules(void)
{
    InitFlagManager();
    InitClientUdp();

    CreateClientThreadPool();
    CreateRecvServerMsgThread();
    CreateClientInterfaceThread();

    return 0;
}

#ifdef __LOCAL_ARGV
static int HandleOneArgv(int argvIndex, const char* argvOne)
{
    // char* s1 = NULL;
    // char* s2 = NULL;

    if (LCstrcasestr(argvOne, "port")) {

    } else {
        DisplayStr("!!!param(argv[%d]:%s) is error", argvIndex, argvOne);
    }

    return 0;
}

static int HandleAllArgv(int argc, const char** argv)
{
    int i = 0;

    for (i = 1; i < argc; i++) {
        HandleOneArgv(i, argv[i]);
    }

    return 0;
}
#endif

static int RecycleResource(void)
{
    DestoryClientThreadPool();

    DestoryCliUdp();
    DestoryFlagManager();

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

// static void* TestThreadPool(void* arg)
// {
//     static uint32_t cnt = 0;

//     DebugDgbout("TestThreadPool success %u", ++cnt);

//     return NULL + 1;
// }

int main(int argc, char const* argv[])
{
#ifdef __LOCAL_ARGV
    HandleAllArgv(argc, argv);
#endif
    ConfigPreInit();
    LocalDbgout("start client");
    InitModules();

    while (1) {
        sleep(5);
        // sleep(1);
        // AddTaskToClientPool(TestThreadPool, NULL);
    }

    RecycleResource();

    return 0;
}

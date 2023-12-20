#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client_info.h"
#include "server.h"
#include "server_dbgout.h"
#include "server_epoll.h"
#include "server_socket.h"

EndID GetServerEndId(void)
{
    static EndID endId = 0;

    return endId;
}

int InitPreModules(void)
{
    GetInitModules()->GetLogFileInfo = GetTcpServerLogFileInfo;
    GetInitModules()->GetEndId = GetServerEndId;

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
    LCPRINTF("start server endid(%d)\n", GetInitModules()->GetEndId());

    InitServerSock();
    InitEpoll();

    CreateEpollProcessThread();
    CreateProcessCliMsgThread();
    while (1) {
        usleep(500);
    }

    CloseFd("serverFd", GetServerFd());
    CloseFd("epFd", GetEpFd());

    return 0;
}

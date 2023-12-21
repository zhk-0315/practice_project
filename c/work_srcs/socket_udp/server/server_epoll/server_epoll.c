#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>

#include "pre_process.h"
#include "safe_api.h"
#include "server_epoll.h"
#include "server_threadpool.h"
#include "server_udp.h"

static int g_epFd = 0;

static void AddFdToSrvEpollWithFd(int fd, uint32_t eventsMask)
{
    int sRet = 0;
    struct epoll_event event = {
        .data.fd = fd,
        .events = eventsMask
    };

    if (!FD_CHECK(g_epFd))
        return;

    sRet = epoll_ctl(g_epFd, EPOLL_CTL_ADD, fd, &event);
    CHECK_FUNCRET_ERR(sRet, -1, "epoll_ctl EPOLL_CTL_ADD error");
}

int InitServerEpoll(void)
{
    if (FD_CHECK(g_epFd))
        return 0;

    g_epFd = epoll_create1(EPOLL_CLOEXEC);
    CHECK_FUNCRET_ERR(g_epFd, -1, "epoll_create1 lcfd error");

    LocalDbgout("init Srv Epoll fd(%d)", g_epFd);

    return 0;
}

int DestoryServerEpoll(void)
{
    if (!FD_CHECK(g_epFd))
        return 0;

    return close(g_epFd);
}

static void* ServerEpollThread(void* arg)
{
    register int i = 0;
    int triggeCnt = 0;
    pthread_t tid = pthread_self();
    struct epoll_event triggeEvents[MAXEVENTS] = { 0 };

    pthread_detach(tid);
    LCpthread_setname_np(tid, "SrvEpoll");

    AddFdToSrvEpollWithFd(STDIN_FILENO, EPOLLIN | EPOLLET);
    AddFdToSrvEpollWithFd(GetSrvUdp()->udpFd, EPOLLIN | EPOLLET);

    while (1) {
        memset(triggeEvents, 0, sizeof(triggeEvents));
        triggeCnt = epoll_wait(g_epFd, triggeEvents, MAXEVENTS, 50);
        if (triggeCnt == -1) {
            LocalDbgout("epoll wait error :%s", strerror(errno));
            continue;
        } else if (0) {
            continue;
        }

        for (i = 0; i < triggeCnt; i++) {
            if (triggeEvents[i].data.fd == STDIN_FILENO) {
                DebugDgbout("stdin in");
                AddTaskToServerPoolReleaseArgMem(ProcessStdin,
                    (void*)&triggeEvents[i].data.fd, sizeof(int));
            } else if (triggeEvents[i].data.fd == GetSrvUdp()->udpFd) {
                DebugDgbout("clis in");
                AddTaskToServerPoolReleaseArgMem(PorcessUdpMsg,
                    (void*)&triggeEvents[i].data.fd, sizeof(int));
            }
        }
    }

    return NULL + 1;
}

int CreateServerEpollThread(void)
{
    int sRet = 0;
    pthread_t tid = 0;

    if (!FD_CHECK(g_epFd))
        InitServerEpoll();

    sRet = pthread_create(&tid, NULL, ServerEpollThread, NULL);
    CHECK_FUNCRET_SUC(sRet, 0, "create ServerEpollThread error");

    LocalDbgout("CreateServerEpollThread complete");

    return 0;
}
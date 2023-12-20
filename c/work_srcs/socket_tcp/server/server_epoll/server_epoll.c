#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "client_info.h"
#include "pre_process.h"
#include "server_dbgout.h"
#include "server_epoll.h"
#include "server_socket.h"

static int epFd = 0;
int GetEpFd(void)
{
    return epFd;
}

int AddEpollByFd(const char* info, int fd, uint32_t eventsMask)
{
    int s32Ret = 0;
    struct epoll_event epEvent = { 0 };

    epEvent.data.fd = fd;
    epEvent.events = eventsMask;
    s32Ret = epoll_ctl(epFd, EPOLL_CTL_ADD, fd, &epEvent);
    if (s32Ret == -1) {
        LocalDbgout("epoll_ctl add %s error: %s", info, strerror(errno));
        return -1;
    }
    DebugDbgout("epoll_ctl add fd(%d) to epfd success", fd);

    return 0;
}

int InitEpoll(void)
{
    epFd = epoll_create1(EPOLL_CLOEXEC);
    if (epFd == -1) {
        LocalDbgout("epoll_create1 error: %s", strerror(errno));
        return -1;
    }

    AddEpollByFd("serverFd", GetServerFd(), EPOLLIN | EPOLLET);
    AddEpollByFd("STDIN_FILENO", STDIN_FILENO, EPOLLIN | EPOLLET);

    DebugDbgout("epFd = %d", epFd);

    return 0;
}

int ProcessEventsByFd(int fd)
{
    if (fd == GetServerFd()) {
        AcceptNewClient();
    } else if (fd == STDIN_FILENO) {
        BroadcastAllClient(fd);
    } else if (GetOneCliInfoByFd(fd) != NULL) {
        ClientMsgEnQueue(fd);
    } else {
        LocalDbgout("no rules this fd");
        return -1;
    }

    DebugDbgout("exec success");

    return 0;
}

int ProcessEpoll(void)
{
    int i = 0;
    int epEventsCnt = -1;
    struct epoll_event epEventsArr[MAXEVENTS] = { 0 };

    prctl(PR_SET_NAME, "SrvEp");

    while (1) {
        memset(epEventsArr, 0, sizeof(epEventsArr));
        epEventsCnt = epoll_wait(epFd, epEventsArr, MAXEVENTS, 50);
        if (epEventsCnt == -1) {
            LocalDbgout("epoll_wait error: %s", strerror(errno));
            continue;
        } else if (epEventsCnt == 0) {
            continue;
        }

        for (i = 0; i < epEventsCnt; i++) {
            ProcessEventsByFd(epEventsArr[i].data.fd);
        }
    }

    return 0;
}

int CreateEpollProcessThread(void)
{
    int s32Ret = -1;
    pthread_t epProcessTid = 0;
    pthread_attr_t epThreadAttr = { 0 };

    s32Ret = pthread_attr_setdetachstate(&epThreadAttr,
        PTHREAD_CREATE_DETACHED);
    if (s32Ret != 0) {
        LocalDbgout("pthread_attr_setdetachstate EpollProcessThread error: %s",
            strerror(errno));
        return -1;
    }

    s32Ret = pthread_create(&epProcessTid, &epThreadAttr,
        (void* (*)(void*))ProcessEpoll, NULL);
    if (s32Ret != 0) {
        LocalDbgout("pthread_create EpollProcessThread error: %s",
            strerror(errno));
        return -1;
    }

    DebugDbgout("CreateEpollProcessThread");

    return 0;
}

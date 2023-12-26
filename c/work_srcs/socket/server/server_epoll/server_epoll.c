#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>

#include "pre_modules.h"
#include "server_epoll.h"
#include "server_pool.h"
#include "server_tcp.h"
#include "server_udp.h"
#include "sock_msg.h"

#define LC_MAXEVENTS (100)

static int g_epfd = 0;

int init_server_epoll(void)
{
    if (CHECK_FD(g_epfd)) {
        return 0;
    }

    g_epfd = epoll_create1(EPOLL_CLOEXEC);
    if (!CHECK_FD(g_epfd)) {
        lc_err_logout("create g_epfd error");
        return -1;
    }

    add_fd_to_srv_epoll(STDIN_FILENO, EPOLLIN | EPOLLET);

    return 0;
}

int destroy_server_epoll(void)
{
    if (!CHECK_FD(g_epfd)) {
        return 0;
    }

    return close(g_epfd);
}

int add_fd_to_srv_epoll(int fd, uint32_t events_mask)
{
    struct epoll_event event_ = {
        .data.fd = fd,
        .events = events_mask
    };

    if (!CHECK_FD(g_epfd)) {
        init_server_epoll();
    }

    return epoll_ctl(g_epfd, EPOLL_CTL_ADD, fd, &event_);
}

int del_fd_from_srv_epoll(int fd)
{
    if (!CHECK_FD(g_epfd)) {
        init_server_epoll();
    }

    return epoll_ctl(g_epfd, EPOLL_CTL_DEL, fd, NULL);
}

static void* process_stdin_msg(void* arg)
{
    int fd = *(int*)arg;
    ssize_t RXsize = 0;
    // lc_msg_package_t msgbuf = { 0 };
    unsigned char RXdata[MSG_DATA_LEN];

    RXsize = read(fd, RXdata, sizeof(RXdata));
    if (RXsize < 0) {
        lc_err_logout("read fd(%d) error", fd);
        return NULL;
    } else if (RXsize == 0) {
        return NULL + 1;
    }

    return NULL + 1;
}

void* server_epoll_thread(void* arg)
{
    int trigge_cnt = 0;
    register int i = 0;
    struct epoll_event events[LC_MAXEVENTS] = { 0 };

    while (1) {
        trigge_cnt = epoll_wait(g_epfd, events, LC_MAXEVENTS, 50);
        if (trigge_cnt < 0) {
            lc_err_logout("epoll_wait g_epfd error");
            continue;
        } else if (trigge_cnt == 0) {
            continue;
        }

        for (i = 0; i < trigge_cnt; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                add_task_to_server_pool_release_arg_mem(process_stdin_msg,
                    &events[i].data.fd, sizeof(int));
            } else if (epoll_trigge_tcpfd(events[i].data.fd)) {
            } else if (epoll_trigge_udpfd(events[i].data.fd)) {
            }
        }
        trigge_cnt = 0;
    }

    return NULL + 1;
}

void create_server_epoll_thread(void)
{
    int iret = 0;
    pthread_t tid = 0;

    iret = pthread_create(&tid, NULL, server_epoll_thread, NULL);
    if (iret) {
        lc_err_logout("create server_epoll_thread error");
    }
}
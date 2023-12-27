#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>

#include "cli_sqlite3.h"
#include "pre_modules.h"
#include "server_epoll.h"
#include "server_pool.h"
#include "server_tcp.h"
#include "server_udp.h"
#include "sock_msg.h"
#include "unsafe_api.h"

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
    lc_msg_package_t msgbuf = { 0 };

    RXsize = read(fd, msgbuf.msg.data, sizeof(msgbuf.msg.data));
    if (RXsize < 0) {
        lc_err_logout("read fd(%d) error", fd);
        return NULL;
    } else if (RXsize == 0) {
        return NULL + 1;
    }

    msgbuf.msg.msg_type = SRV_BROADCAST;
    msgbuf.msg.srcid = read_pre_modules_addr()->endid;
    traverse_table_send_msg(&msgbuf);

    return NULL + 1;
}

static void* process_accept_tcp_link(void* arg)
{
    int fd = *(int*)arg;
    int newfd = 0;

    newfd = accept(fd, NULL, (socklen_t*)&g_addrlen);
    if (!CHECK_FD(newfd)) {
        lc_err_logout("accept newfd error");
        return NULL;
    }

    add_fd_to_srv_epoll(newfd, EPOLLIN | EPOLLET);

    return NULL + 1;
}

static void* process_recv_cli_msg(void* arg)
{
    int fd = *(int*)arg;

    if (epoll_trigge_udpfd(fd)) {
        recv_udp_cli_msg(fd);
    } else {
        recv_tcp_cli_msg(fd);
    }

    return NULL + 1;
}

static void* server_epoll_thread(void* arg)
{
    int trigge_cnt = 0;
    register int i = 0;
    pthread_t tid = pthread_self();
    struct epoll_event events[LC_MAXEVENTS] = { 0 };

    pthread_detach(tid);
    lc_pthread_setname_np(tid, "SrvEpoll");

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
                add_task_to_server_pool_release_arg_mem(process_accept_tcp_link,
                    &events[i].data.fd, sizeof(int));
            } else {
                add_task_to_server_pool_release_arg_mem(process_recv_cli_msg,
                    &events[i].data.fd, sizeof(int));
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
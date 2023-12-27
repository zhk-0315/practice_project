#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "cli_sqlite3.h"
#include "pre_modules.h"
#include "server_epoll.h"
#include "server_msgqueue.h"
#include "sock_msg.h"

static int g_tcpfd = 0;

int init_server_tcp(void)
{
    int iret = 0;
    int optval_ = 1;
    struct sockaddr_in lcaddr = { 0 };

    if (CHECK_FD(g_tcpfd)) {
        return 0;
    }

    g_tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!CHECK_FD(g_tcpfd)) {
        lc_err_logout("socket g_tcpfd error");
        return -1;
    }

    iret = setsockopt(g_tcpfd, SOL_SOCKET, SO_REUSEADDR, &optval_, sizeof(optval_));
    if (iret != 0) {
        lc_err_logout("setsockopt g_tcpfd error");
    }

    lcaddr.sin_family = AF_INET;
    lcaddr.sin_addr.s_addr = INADDR_ANY;
    lcaddr.sin_port = htons(g_server_tcp_port);
    iret = bind(g_tcpfd, (struct sockaddr*)&lcaddr, g_addrlen);
    if (iret != 0) {
        lc_err_logout("bind g_tcpfd error");
        return -1;
    }

    iret = listen(g_tcpfd, 3);
    if (iret != 0) {
        lc_err_logout("listen g_tcpfd error");
        return -1;
    }

    add_fd_to_srv_epoll(g_tcpfd, EPOLLIN | EPOLLET);

    return 0;
}

int destroy_server_tcp(void)
{
    if (!CHECK_FD(g_tcpfd)) {
        return 0;
    }

    del_fd_from_srv_epoll(g_tcpfd);

    return close(g_tcpfd);
}

_Bool epoll_trigge_tcpfd(int fd)
{
    return (fd == g_tcpfd);
}

int send_msg_by_tcp(lc_msg_package_t* msg_pack, int cli_fd)
{
    ssize_t TXsize = 0;

    TXsize = send(cli_fd, msg_pack, sizeof(lc_msg_package_t), 0);
    if (TXsize != sizeof(lc_msg_package_t)) {
        lc_err_logout("send to endid(%d) error", msg_pack->msg.destid);
        return -1;
    }

    return 0;
}

int recv_tcp_cli_msg(int fd)
{
    lc_msg_package_t msgbuf = { 0 };
    ssize_t RXsize = 0;

    RXsize = recv(fd, &msgbuf, sizeof(lc_msg_package_t), 0);
    if (RXsize < 0) {
        lc_err_logout("recv fd(%d) error", fd);
        return -1;
    } else if (RXsize == 0) {
        del_fd_from_srv_epoll(fd);
        del_tcp_cli_from_database_by_fd(fd);
        close(fd);
        return 0;
    }

    if (msgbuf.msg.msg_type == SAVE_CLI_INFO) {
        add_tcp_cli_to_database(msgbuf.msg.srcid, fd);
    } else {
        en_srv_msg_queue(&msgbuf);
    }

    return 0;
}

int send_tcp_cli_msg(socket_handle_t* _socket, lc_msg_package_t* _msgbuf)
{
    ssize_t TXsize = 0;

    if (_socket->socket_type != TCP_LINK) {
        return -1;
    }

    TXsize = send(_socket->socket_handle.fd, _msgbuf,
        sizeof(lc_msg_package_t), 0);
    if (TXsize != sizeof(lc_msg_package_t)) {
        lc_err_logout("send msg error!");
    }

    return 0;
}
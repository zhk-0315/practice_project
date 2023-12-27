#include <pthread.h>

#include "cli_sqlite3.h"
#include "pre_modules.h"
#include "server_msgqueue.h"
#include "server_pool.h"
#include "server_socket.h"
#include "server_tcp.h"
#include "server_udp.h"
#include "sock_msg.h"
#include "unsafe_api.h"

static void* forward_msg_clients(void* arg)
{
    lc_msg_package_t* _msgbuf = (lc_msg_package_t*)arg;
    socket_handle_t socket = { 0 };

    get_cli_by_endid(_msgbuf->msg.destid, &socket);
    switch (socket.socket_type) {
    case TCP_LINK:
        send_tcp_cli_msg(&socket, _msgbuf);
        break;
    case UDP_LINK:
        send_udp_cli_msg(&socket, _msgbuf);
        break;
    default:
        break;
    }

    return NULL + 1;
}

void* socket_msg_process_thread(void* arg)
{
    lc_msg_package_t msgbuf = { 0 };
    pthread_t tid = pthread_self();

    pthread_detach(tid);
    lc_pthread_setname_np(tid, "MsgProcess");

    while (1) {
        if (de_srv_msg_queue(&msgbuf) != QUEUE_SUCCESS) {
            usleep(10000);
            continue;
        }

        if (msgbuf.msg.destid == read_pre_modules_addr()->endid) {
            add_task_to_server_pool_release_arg_mem(dispaly_msg_package,
                &msgbuf, sizeof(lc_msg_package_t));
        } else {
            add_task_to_server_pool_release_arg_mem(forward_msg_clients,
                &msgbuf, sizeof(lc_msg_package_t));
        }
    }

    return NULL + 1;
}

void create_socket_msg_process_thread(void)
{
    int iret = 0;
    pthread_t tid = 0;

    iret = pthread_create(&tid, NULL, socket_msg_process_thread, NULL);
    if (iret) {
        lc_err_logout("create socket_msg_process_thread error");
    }
}
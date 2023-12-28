#ifndef __CLIENT__CLIENT_SOCKET__CLIENT_SOCKET_H
#define __CLIENT__CLIENT_SOCKET__CLIENT_SOCKET_H

#include "client_tcp.h"
#include "client_udp.h"

typedef enum socket_type_t {
    UNSET,
    TCP_LINK,
    UDP_LINK
} socket_type_t;

int switch_socket_type(void);
char* display_cur_socket_type(void);
int send_msg_to_server(void);
int send_msg_to_other_cli(void);

static inline void destroy_socket(void)
{
    try_destroy_client_udp();
    try_destroy_client_tcp();
}

#endif
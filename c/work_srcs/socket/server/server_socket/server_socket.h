#ifndef __SERVER__SERVER_SOCKET__SERVER_SOCKET_H
#define __SERVER__SERVER_SOCKET__SERVER_SOCKET_H

#include <netinet/in.h>

typedef enum socket_type_t {
    UNSET,
    TCP_LINK,
    UDP_LINK
} socket_type_t;

typedef struct socket_handle_t {
    socket_type_t socket_type;
    union {
        int fd;
        struct sockaddr_in addr;
    } socket_handle;
} socket_handle_t;

void create_socket_msg_process_thread(void);

#endif
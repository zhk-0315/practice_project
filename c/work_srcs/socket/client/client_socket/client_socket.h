#ifndef __CLIENT__CLIENT_SOCKET__CLIENT_SOCKET_H
#define __CLIENT__CLIENT_SOCKET__CLIENT_SOCKET_H

typedef enum socket_type_t {
    UNSET,
    TCP_LINK,
    UDP_LINK
} socket_type_t;

int create_or_switch_socket_type(socket_type_t socket_type);
int destroy_socket(void);

#endif
#include "client_socket.h"
#include "client_tcp.h"
#include "client_udp.h"

int create_or_switch_socket_type(socket_type_t socket_type)
{
    switch (socket_type) {
    case TCP_LINK:
        try_destroy_client_udp();
        init_client_tcp();
        break;
    case UDP_LINK:
        try_destroy_client_tcp();
        init_client_udp();
        break;
    default:
        break;
    }

    return 0;
}

int destroy_socket(void)
{
    try_destroy_client_udp();
    try_destroy_client_tcp();

    return 0;
}
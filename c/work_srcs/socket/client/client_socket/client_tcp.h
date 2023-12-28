#ifndef __CLIENT__CLIENT_SOCKET__CLIENT_TCP_H
#define __CLIENT__CLIENT_SOCKET__CLIENT_TCP_H

#include "sock_msg.h"

int init_client_tcp(void);
int try_destroy_client_tcp(void);
void* send_msg_by_tcp(void* arg);

#endif
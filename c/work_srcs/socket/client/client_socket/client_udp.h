#ifndef __CLIENT__CLIENT_SOCKET__CLIENT_UDP_H
#define __CLIENT__CLIENT_SOCKET__CLIENT_UDP_H

int init_client_udp(void);
int try_destroy_client_udp(void);
void* send_msg_by_udp(void* arg);
void* recv_msg_by_udp(void* arg);

#endif
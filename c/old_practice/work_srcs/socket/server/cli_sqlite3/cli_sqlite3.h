#ifndef SERVER__CLI_SQLITE3__CLI_SQLITE3_H
#define SERVER__CLI_SQLITE3__CLI_SQLITE3_H

#include "pre_modules.h"
#include "server_socket.h"
#include "sock_msg.h"

int create_cli_sql_database(void);
int destroy_cli_sql_database(void);
int add_tcp_cli_to_database(endid_t endid, int fd);
int del_tcp_cli_from_database_by_fd(int fd);
int add_udp_cli_to_database(endid_t endid, uint32_t ipaddr, uint16_t port);
int del_udp_cli_from_database_by_ipaddr(uint32_t ipaddr, uint16_t port);
int get_cli_by_endid(endid_t ednid, socket_handle_t* _socket_mem);
int traverse_table_send_msg(lc_msg_package_t* msg_pack);

#endif
#include <pthread.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pre_modules.h"
#include "server_socket.h"
#include "server_tcp.h"
#include "server_udp.h"
#include "sock_msg.h"

typedef struct lc_sql_t {
    sqlite3* sql;
    pthread_mutex_t mutex_;
} lc_sql_t;

static lc_sql_t lc_sqls = {
    .sql = NULL,
    .mutex_ = PTHREAD_MUTEX_INITIALIZER
};

int create_cli_sql_database(void)
{
    int iret = 0;
    char sql_fpath[256] = { 0 };
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (lc_sqls.sql) {
        return 0;
    }

    getcwd(sql_fpath, sizeof(sql_fpath));
    strcat(sql_fpath, "/srv_sql.db");

    if (!access(sql_fpath, F_OK)) {
        remove(sql_fpath);
    }

    iret = sqlite3_open(sql_fpath, &lc_sqls.sql);
    if (iret != SQLITE_OK) {
        lc_logout("sqlite3_open database error");
    }

    sprintf(sql_cmd, "CREATE TABLE IF NOT EXISTS CliInfo"
                     "(endid int PRIMARY KEY NOT NULL, "
                     "fd int, ipaddr uint32_t, port uint16_t);");
    iret = sqlite3_exec(lc_sqls.sql, sql_cmd, NULL, NULL, (char**)&sql_err);
    if (iret != SQLITE_OK) {
        lc_logout("CREATE TABLE CliInfo error: %s", sql_err);
    }

    return 0;
}

int destroy_cli_sql_database(void)
{
    if (!lc_sqls.sql) {
        pthread_mutex_destroy(&lc_sqls.mutex_);
        return 0;
    }

    pthread_mutex_lock(&lc_sqls.mutex_);
    sqlite3_close(lc_sqls.sql);
    pthread_mutex_unlock(&lc_sqls.mutex_);
    pthread_mutex_destroy(&lc_sqls.mutex_);

    return 0;
}

int add_tcp_cli_to_database(endid_t endid, int fd)
{
    int iret = 0;
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (!lc_sqls.sql) {
        create_cli_sql_database();
    }

    pthread_mutex_lock(&lc_sqls.mutex_);
    sprintf(sql_cmd, "INSERT INTO CliInfo VALUES (%d, %d, NULL, NULL);",
        endid, fd);
    iret = sqlite3_exec(lc_sqls.sql, sql_cmd, NULL, NULL, (char**)&sql_err);
    if (iret != SQLITE_OK) {
        lc_logout("INSERT TO CliInfo error: %s", sql_err);
    }
    if (iret == SQLITE_CONSTRAINT) {
        sprintf(sql_cmd, "UPDATE CliInfo SET fd=%d,ipaddr=null,port=null "
                         "WHERE endid == %d;",
            fd, endid);
        iret = sqlite3_exec(lc_sqls.sql, sql_cmd, NULL, NULL, (char**)&sql_err);
        if (iret != SQLITE_OK) {
            lc_logout("UPDATE CliInfo error: %s", sql_err);
        }
    }

    pthread_mutex_unlock(&lc_sqls.mutex_);

    return 0;
}

int del_tcp_cli_from_database_by_fd(int fd)
{
    int iret = 0;
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (!lc_sqls.sql) {
        create_cli_sql_database();
    }

    pthread_mutex_lock(&lc_sqls.mutex_);
    sprintf(sql_cmd, "DELETE FROM CliInfo WHERE fd == %d;", fd);
    iret = sqlite3_exec(lc_sqls.sql, sql_cmd, NULL, NULL, (char**)&sql_err);
    if (iret != SQLITE_OK) {
        lc_logout("DELETE FROM CliInfo error: %s", sql_err);
    }
    pthread_mutex_unlock(&lc_sqls.mutex_);

    return 0;
}

int add_udp_cli_to_database(endid_t endid, uint32_t ipaddr, uint16_t port)
{
    int iret = 0;
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (!lc_sqls.sql) {
        create_cli_sql_database();
    }

    lc_debug_logout("endid(%d),ipaddr(%u),port(%u)", endid, ipaddr, port);

    pthread_mutex_lock(&lc_sqls.mutex_);
    sprintf(sql_cmd, "INSERT INTO CliInfo VALUES (%d, NULL, %u, %u);",
        endid, ipaddr, port);
    iret = sqlite3_exec(lc_sqls.sql, sql_cmd, NULL, NULL, (char**)&sql_err);
    lc_debug_logout("iret= %d", iret);
    if (iret != SQLITE_OK) {
        lc_logout("INSERT TO CliInfo error: %s", sql_err);
    }
    if (iret == SQLITE_CONSTRAINT) {
        sprintf(sql_cmd, "UPDATE CliInfo SET fd = NULL,addr = %u,port = %u "
                         "WHERE endid == %d;",
            ipaddr, port, endid);
        iret = sqlite3_exec(lc_sqls.sql, sql_cmd, NULL, NULL, (char**)&sql_err);
        lc_debug_logout("iret= %d", iret);
        if (iret != SQLITE_OK) {
            lc_logout("UPDATE CliInfo error: %s", sql_err);
        }
    }
    pthread_mutex_unlock(&lc_sqls.mutex_);

    return 0;
}

int del_udp_cli_from_database_by_ipaddr(uint32_t ipaddr, uint16_t port)
{
    int iret = 0;
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (!lc_sqls.sql) {
        create_cli_sql_database();
    }

    pthread_mutex_lock(&lc_sqls.mutex_);
    sprintf(sql_cmd, "DELETE FROM CliInfo WHERE ipaddr == %u AND port == %u;",
        ipaddr, port);
    iret = sqlite3_exec(lc_sqls.sql, sql_cmd, NULL, NULL, (char**)&sql_err);
    if (iret != SQLITE_OK) {
        lc_logout("DELETE FROM CliInfo error: %s", sql_err);
    }
    pthread_mutex_unlock(&lc_sqls.mutex_);

    return 0;
}

static int select_result_handler(void* arg, int retc, char** retv, char** retname)
{
    socket_handle_t* _socket_mem = (socket_handle_t*)arg;

    if (retc != 4 || !_socket_mem) {
        return SQLITE_ERROR;
    }

    if (_socket_mem->socket_type != UNSET) {
        return SQLITE_OK;
    }

    if (retv[1] && !retv[2] && !retv[3]) {
        _socket_mem->socket_type = TCP_LINK;
        _socket_mem->socket_handle.fd = atoi(retv[1]);
    } else if (!retv[1] && retv[2] && retv[3]) {
        _socket_mem->socket_type = UDP_LINK;
        _socket_mem->socket_handle.addr.sin_family = AF_INET;
        _socket_mem->socket_handle.addr.sin_addr.s_addr = (uint32_t)atoi(retv[2]);
        _socket_mem->socket_handle.addr.sin_port = (uint16_t)atoi(retv[3]);
    }

    return SQLITE_OK;
}

int get_cli_by_endid(endid_t ednid, socket_handle_t* _socket_mem)
{
    int iret = 0;
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (!lc_sqls.sql) {
        create_cli_sql_database();
    }

    pthread_mutex_lock(&lc_sqls.mutex_);
    sprintf(sql_cmd, "SELECT * FROM CliInfo WHERE endid == %d;", ednid);
    iret = sqlite3_exec(lc_sqls.sql, sql_cmd, select_result_handler,
        _socket_mem, (char**)&sql_err);
    if (iret != SQLITE_OK) {
        lc_logout("SELECT FROM TO CliInfo error: %s", sql_err);
    }
    pthread_mutex_unlock(&lc_sqls.mutex_);

    return 0;
}

static int send_msg_to_all_ret(void* arg, int retc, char** retv, char** retname)
{
    int cli_fd = 0;
    struct sockaddr_in lcaddr = { 0 };
    lc_msg_package_t* msg_pack = (lc_msg_package_t*)arg;

    if (retc != 4 || !msg_pack) {
        return SQLITE_ERROR;
    }

    msg_pack->msg.destid = atoi(retv[0]);
    if (retv[1] && !retv[2] && !retv[3]) {
        cli_fd = atoi(retv[1]);
        send_msg_by_tcp(msg_pack, cli_fd);
    } else if (!retv[1] && retv[2] && retv[3]) {
        lcaddr.sin_family = AF_INET;
        lcaddr.sin_addr.s_addr = (uint32_t)atoi(retv[2]);
        lcaddr.sin_port = (uint16_t)atoi(retv[3]);
        send_msg_by_udp(msg_pack, &lcaddr);
    }

    return SQLITE_OK;
}

int traverse_table_send_msg(lc_msg_package_t* msg_pack)
{
    int iret = 0;
    char sql_err[256] = { 0 };
    char sql_cmd[256] = { 0 };

    if (!lc_sqls.sql) {
        create_cli_sql_database();
    }

    pthread_mutex_lock(&lc_sqls.mutex_);
    sprintf(sql_cmd, "SELECT * FROM CliInfo;");
    iret = sqlite3_exec(lc_sqls.sql, sql_cmd, send_msg_to_all_ret,
        msg_pack, (char**)&sql_err);
    if (iret != SQLITE_OK) {
        lc_logout("SELECT FROM TO CliInfo error: %s", sql_err);
    }
    pthread_mutex_unlock(&lc_sqls.mutex_);

    return 0;
}
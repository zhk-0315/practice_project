#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "pre_modules.h"

const socklen_t g_addrlen = sizeof(struct sockaddr);
const endid_t g_server_endid = 0;
const char* g_server_addr = "127.0.0.1";
const uint16_t g_server_udp_port = 36666;
const uint16_t g_server_tcp_port = 36667;

static pre_modules_t* init_pre_modules(void)
{
    static pre_modules_t* modules = NULL;

    if (modules) {
        return modules;
    }

    modules = malloc(sizeof(pre_modules_t));

    return modules;
}

void destroy_pre_modules(void)
{
    free(init_pre_modules());
}

pre_modules_t* write_pre_modules_addr(void)
{
    return init_pre_modules();
}

const pre_modules_t* read_pre_modules_addr(void)
{
    return (const pre_modules_t*)init_pre_modules();
}
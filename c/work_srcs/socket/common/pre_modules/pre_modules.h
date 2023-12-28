#ifndef __COMMON__PRE_MODULES__PRE_MODULES_H
#define __COMMON__PRE_MODULES__PRE_MODULES_H

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "logout.h"

typedef pid_t endid_t;

typedef struct pre_modules_t {
    log_file_t file_;
    endid_t endid;
} pre_modules_t;

void destroy_pre_modules(void);
pre_modules_t* write_pre_modules_addr(void);
const pre_modules_t* read_pre_modules_addr(void);

#define lc_logout(fmt, args...)                               \
    do {                                                      \
        logout(&read_pre_modules_addr()->file_, fmt, ##args); \
    } while (0)

#define lc_err_logout(fmt, args...)                                              \
    do {                                                                         \
        char lc_err_str[256] = { 0 };                                            \
        strerror_r(errno, lc_err_str, sizeof(lc_err_str));                       \
        logout(&read_pre_modules_addr()->file_, fmt ": %s", ##args, lc_err_str); \
    } while (0)

#define lc_printf(fmt, args...) \
    do {                        \
        printf(fmt, ##args);    \
        fflush(stdout);         \
    } while (0)

#define lc_display_str(fmt, args...) \
    do {                             \
        lc_printf(fmt "\n", ##args); \
    } while (0)

#define lc_scanf(fmt, args...)        \
    do {                              \
        scanf(fmt, ##args);           \
        while (getchar() != '\n') { } \
    } while (0)

#define CHECK_FD(macro_fd) (macro_fd > STDERR_FILENO)

extern const socklen_t g_addrlen;
extern const endid_t g_server_endid;
extern const char* g_server_addr;
extern const uint16_t g_server_udp_port;
extern const uint16_t g_server_tcp_port;

#endif
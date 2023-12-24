#ifndef __COMMON__PRE_MODULES__PRE_MODULES_H
#define __COMMON__PRE_MODULES__PRE_MODULES_H

#include <stdio.h>
#include <sys/types.h>

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

#define lc_printf(fmt, args...) \
    do {                        \
        printf(fmt, ##args);    \
        fflush(stdout);         \
    } while (0)

#define lc_scanf(fmt, args...)        \
    do {                              \
        scanf(fmt, ##args);           \
        while (getchar() != '\n') { } \
    } while (0)

#endif
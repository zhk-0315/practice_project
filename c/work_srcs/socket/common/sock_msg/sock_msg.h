#ifndef __COMMON__SOCK_MSG__SOCK_MSG_H
#define __COMMON__SOCK_MSG__SOCK_MSG_H

#include "pre_modules.h"

typedef enum lc_msg_type_t {
    SAVE_CLI_INFO,
    TCP_DATA,
    UDP_DATA,
    SRV_BROADCAST
} lc_msg_type_t;

#define MSG_DATA_LEN (512)
typedef union lc_msg_package_t {
    struct msg {
        lc_msg_type_t msg_type;
        endid_t srcid;
        endid_t destid;
        unsigned char data[MSG_DATA_LEN];
    } msg;
    unsigned char buf[sizeof(struct msg) + 1];
} lc_msg_package_t;

#endif
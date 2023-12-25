#ifndef __COMMON__SOCK_MSG__MSG_QUEUE__MSG_QUEUE_H
#define __COMMON__SOCK_MSG__MSG_QUEUE__MSG_QUEUE_H

#include "list.h"
#include "sock_msg.h"

typedef struct msg_queue_node_t {
    lc_msg_package_t msg_package;
    struct list_head list;
} msg_queue_node_t;

#ifndef LC_QUEUE_RET
#define LC_QUEUE_RET
typedef enum queue_ret_t {
    QUEUE_SUCCESS,
    QUEUE_ERROR,
    QUEUE_EMPTY,
    QUEUE_FULL
} queue_ret_t;
#endif

struct list_head* init_msg_queue(void);
queue_ret_t en_msg_to_queue(struct list_head* msg_queue, const lc_msg_package_t* _msg_package);
queue_ret_t de_msg_from_queue(struct list_head* msg_queue, lc_msg_package_t* _msg_package);
queue_ret_t destroy_msg_queue(struct list_head** _msg_queue);

#endif
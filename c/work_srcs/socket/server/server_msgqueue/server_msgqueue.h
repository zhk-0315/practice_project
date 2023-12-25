#ifndef __SERVER__SERVER_MSGQUEUE__SERVER_MSGQUEUE_H
#define __SERVER__SERVER_MSGQUEUE__SERVER_MSGQUEUE_H

#include <pthread.h>

#include "msg_queue.h"

typedef struct srv_msg_queue_t {
    struct list_head* queue;
    pthread_mutex_t mutex_;
} srv_msg_queue_t;

queue_ret_t en_srv_msg_queue(const lc_msg_package_t* _msg_package);
queue_ret_t de_srv_msg_queue(lc_msg_package_t* _msg_package);
queue_ret_t destroy_srv_msg_queue(void);

#endif
#include <stdio.h>

#include "server_msgqueue.h"

static srv_msg_queue_t srv_msg_queue = {
    .queue = NULL,
    .mutex_ = PTHREAD_MUTEX_INITIALIZER
};

static queue_ret_t init_srv_msg_queue(void)
{
    if (srv_msg_queue.queue) {
        return QUEUE_SUCCESS;
    }

    pthread_mutex_lock(&srv_msg_queue.mutex_);
    srv_msg_queue.queue = init_msg_queue();
    pthread_mutex_unlock(&srv_msg_queue.mutex_);

    return QUEUE_SUCCESS;
}

queue_ret_t en_srv_msg_queue(const lc_msg_package_t* _msg_package)
{
    queue_ret_t ret = 0;

    if (!srv_msg_queue.queue) {
        init_srv_msg_queue();
    }

    pthread_mutex_lock(&srv_msg_queue.mutex_);
    ret = en_msg_to_queue(srv_msg_queue.queue, _msg_package);
    pthread_mutex_unlock(&srv_msg_queue.mutex_);

    return ret;
}

queue_ret_t de_srv_msg_queue(lc_msg_package_t* _msg_package)
{
    queue_ret_t ret = 0;

    if (!srv_msg_queue.queue) {
        init_srv_msg_queue();
    }

    pthread_mutex_lock(&srv_msg_queue.mutex_);
    ret = de_msg_from_queue(srv_msg_queue.queue, _msg_package);
    pthread_mutex_unlock(&srv_msg_queue.mutex_);

    return ret;
}

queue_ret_t destroy_srv_msg_queue(void)
{
    queue_ret_t ret = 0;

    if (!srv_msg_queue.queue) {
        pthread_mutex_destroy(&srv_msg_queue.mutex_);
        return QUEUE_SUCCESS;
    }

    pthread_mutex_lock(&srv_msg_queue.mutex_);
    ret = destroy_msg_queue(&srv_msg_queue.queue);
    pthread_mutex_unlock(&srv_msg_queue.mutex_);
    pthread_mutex_destroy(&srv_msg_queue.mutex_);

    return ret;
}
#include <stdio.h>
#include <stdlib.h>

#include "msg_queue.h"

struct list_head* init_msg_queue(void)
{
    struct list_head* queue = (struct list_head*)malloc(sizeof(struct list_head));

    INIT_LIST_HEAD(queue);

    return queue;
}

queue_ret_t en_msg_to_queue(struct list_head* msg_queue, const lc_msg_package_t* _msg_package)
{
    msg_queue_node_t* node = NULL;

    if (!msg_queue || !_msg_package) {
        return QUEUE_ERROR;
    }

    node = (msg_queue_node_t*)malloc(sizeof(msg_queue_node_t));
    node->msg_package = *_msg_package;
    list_add(&node->list, msg_queue);

    return QUEUE_SUCCESS;
}

queue_ret_t de_msg_from_queue(struct list_head* msg_queue, lc_msg_package_t* _msg_package)
{
    msg_queue_node_t* node = NULL;

    if (!msg_queue) {
        return QUEUE_ERROR;
    }

    if (list_empty(msg_queue)) {
        return QUEUE_EMPTY;
    }

    node = list_entry(msg_queue->next, msg_queue_node_t, list);
    if (_msg_package) {
        *_msg_package = node->msg_package;
    }
    list_del(&node->list);

    free(node);

    return QUEUE_SUCCESS;
}

queue_ret_t destroy_msg_queue(struct list_head** _msg_queue)
{
    struct list_head* msg_queue = *_msg_queue;

    if (!msg_queue) {
        return QUEUE_SUCCESS;
    }

    while (de_msg_from_queue(msg_queue, NULL) == QUEUE_SUCCESS) { }

    free(msg_queue);
    *_msg_queue = NULL;

    return QUEUE_SUCCESS;
}

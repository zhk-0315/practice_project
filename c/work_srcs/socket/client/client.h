#ifndef __CLIENT__CLIENT_H
#define __CLIENT__CLIENT_H

#include <pthread.h>
#include <stdint.h>

typedef struct cli_flag_t {
    uint8_t init_cli_info : 1;
} cli_flag_t;

typedef struct cli_flag_manager_t {
    cli_flag_t flag;
    pthread_mutex_t mutex_;
} cli_flag_manager_t;

cli_flag_manager_t* get_cli_flag(void);

#endif
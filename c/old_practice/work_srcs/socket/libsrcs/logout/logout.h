#ifndef __LIBSRCS__LOGOUT__LOGOUT_H
#define __LIBSRCS__LOGOUT__LOGOUT_H

typedef enum log_file_status_t {
    ALWAYS_OPEN,
    EVERY_TIME_OPEN
} log_file_status_t;

#define LOG_FILE_PATH_LEN (128)
typedef struct log_file_t {
    log_file_status_t status;
    char path[LOG_FILE_PATH_LEN];
} log_file_t;

void init_log_file_clear(_Bool init_clear_file, long max_log_size);
void __logout(const log_file_t* file, const char* fmt, ...);

#define notdir_fpath(fpath) ({            \
    char* tmp = strrchr(fpath, '/');      \
    (const char*)(tmp ? tmp + 1 : fpath); \
})

#define logout(file, fmt, args...)                                   \
    do {                                                             \
        __logout(file, "[file:%s, func:%s, line:%d]" fmt "\n",       \
            notdir_fpath(__FILE__), __FUNCTION__, __LINE__, ##args); \
    } while (0)

#endif
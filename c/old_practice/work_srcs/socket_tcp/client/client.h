#ifndef __CLIENT_CLIENT_H_
#define __CLIENT_CLIENT_H_

#include <stdint.h>

typedef struct FlagManager {
    uint8_t initCliInfo : 1;
} FlagManager;

FlagManager* GetFlagManager(void);

#endif
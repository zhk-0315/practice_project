#include <stdio.h>
#include <stdlib.h>

#include "pre_modules.h"

static pre_modules_t* init_pre_modules(void)
{
    static pre_modules_t* modules = NULL;

    if (modules) {
        return modules;
    }

    modules = malloc(sizeof(pre_modules_t));

    return modules;
}

void destroy_pre_modules(void)
{
    free(init_pre_modules());
}

pre_modules_t* write_pre_modules_addr(void)
{
    return init_pre_modules();
}

const pre_modules_t* read_pre_modules_addr(void)
{
    return (const pre_modules_t*)init_pre_modules();
}
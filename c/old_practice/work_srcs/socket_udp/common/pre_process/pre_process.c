#include <stdio.h>

#include "pre_process.h"

ModulesManager* ToSetModulesManager(void)
{
    static ModulesManager modulesManager = { 0 };

    return &modulesManager;
}

const ModulesManager* GetModulesManager(void)
{
    return ToSetModulesManager();
}
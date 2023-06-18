#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <SPIFFS.h>
#include "oled.h"

#define SPIFFS_INIT_FAILURE "Failed to initiate SPIFFS"
#define SPIFFS_INIT_SUCCESS "Initiated SPIFFS"

class FILE_MANAGER
{
public:
    static void initSPIFFS();
};

void FILE_MANAGER::initSPIFFS()
{
    if (!SPIFFS.begin())
    {
        OLED::write(SPIFFS_INIT_FAILURE);
        for (;;)
            ;
    }

    OLED::write(SPIFFS_INIT_SUCCESS);
};

#endif
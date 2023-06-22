#pragma once
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include <SPIFFS.h>
#include <string>
#include "oled.h"

#define SPIFFS_INIT_FAILURE "Failed to initiate SPIFFS"
#define SPIFFS_INIT_SUCCESS "Initiated SPIFFS"

#define BACKUP_FILE_WRITTEN_SUCCESS "Backup file written successfuly: "
#define BACKUP_FILE_WRITTEN_EMPTY_STRING "Backup file written empty string"
#define BACKUP_FILE_WRITTEN_FAILURE "Backup file write failed"

class FILE_MANAGER
{
public:
    static void initSPIFFS();
    static void readFile(const std::string &path, char *buffer);
    static void overwriteFile(const std::string &path, const std::string &content);
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

void FILE_MANAGER::readFile(const std::string &path, char *buffer)
{
    File file = SPIFFS.open(path.c_str(), FILE_READ);

    size_t size = file.size();
    size_t bytes = file.readBytes(buffer, size);

    file.close();
};

void FILE_MANAGER::overwriteFile(const std::string &path, const std::string &content)
{
    File file = SPIFFS.open(path.c_str(), FILE_WRITE);
    size_t length = content.length();

    if (file.print(length > 0 ? content.c_str() : ""))
    {
        std::string s = std::string(BACKUP_FILE_WRITTEN_SUCCESS) + content;
        Serial.println(s.c_str());
    }
    else
    {
        if (length == 0)
        {
            Serial.println(BACKUP_FILE_WRITTEN_EMPTY_STRING);
        }
        else
        {
            Serial.println(BACKUP_FILE_WRITTEN_FAILURE);
        }
    }

    file.close();
};

#endif
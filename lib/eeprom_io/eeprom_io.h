
#pragma once
#ifndef EEPROM_IO_H
#define EEPROM_IO_H
#include <EEPROM.h>
#include <string>
#include "file_manager.h"
#include "s_utils.h"
#include "oled.h"
#include "env.h"

#define INITIATED_EEPROM "Initiated EEPROM"
const int SCHEDULE_ADDRESS = 0;

class EEPROM_IO
{
public:
    static void readSchedule(char *buffer);
    static void initEEPROM();
    static void appendSchedule(const std::string &s);
    static void overwriteSchedule(const std::string &s);
    static void deleteFromSchedule(const std::string &s);
};

void EEPROM_IO::readSchedule(char *buffer)
{

    for (size_t i = 0; i < EEPROM_SIZE; i++)
    {
        buffer[i] = EEPROM.read(SCHEDULE_ADDRESS + i);
        if (buffer[i] == '\0')
        {
            break;
        }
    }
};

void EEPROM_IO::initEEPROM()
{
    EEPROM.begin(EEPROM_SIZE);
    OLED::write(INITIATED_EEPROM);

    if (!RESET_EEPROM_ON_STARTUP)
        return;

    char fileBuffer[EEPROM_SIZE];
    FILE_MANAGER::readFile(BACKUP_FILE_PATH, fileBuffer);

    std::string str(fileBuffer);

    bool shouldOverwrite = str.length() > 0 &&
                           !S_UTILS::startsWith(str, DAILY) &&
                           !S_UTILS::startsWith(str, WEEKLY);

    if (shouldOverwrite)
    {
        overwriteSchedule(str);
    };

    char readBuffer[EEPROM_SIZE];
    readSchedule(readBuffer);
    std::string sep = ": ";
    std::string s = INITIATED_EEPROM + sep + std::string(readBuffer);
    Serial.println(s.c_str());
};

void EEPROM_IO::overwriteSchedule(const std::string &s)
{
    std::string content = s.c_str() + '\0';
    size_t length = content.length();

    for (size_t i = 0; i < EEPROM_SIZE; i++)
    {
        EEPROM.write(SCHEDULE_ADDRESS + i, 0);
    }

    for (size_t i = 0; i < length; i++)
    {
        EEPROM.write(SCHEDULE_ADDRESS + i, content[i]);
    }

    EEPROM.commit();
    FILE_MANAGER::overwriteFile(BACKUP_FILE_PATH, content);
};

void EEPROM_IO::appendSchedule(const std::string &s)
{
    char buffer[EEPROM_SIZE];
    readSchedule(buffer);
    std::string formatted = S_UTILS::removeNullTerminators(buffer).c_str();
    overwriteSchedule(formatted + s.c_str());
};

void EEPROM_IO::deleteFromSchedule(const std::string &s)
{
    char buffer[EEPROM_SIZE];
    readSchedule(buffer);
    std::string formatted = S_UTILS::removeNullTerminators(buffer).c_str();
    std::string result = S_UTILS::replaceSubstr(formatted, s, "");
    overwriteSchedule(result.c_str());
};

#endif
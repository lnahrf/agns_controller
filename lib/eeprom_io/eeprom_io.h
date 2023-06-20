
#pragma once
#ifndef EEPROM_IO_H
#define EEPROM_IO_H
#include <EEPROM.h>
#include <string>
#include "s_utils.h"
#include "oled.h"
#include "env.h"

#define INITIATED_EEPROM "Initiated EEPROM"
const int SCHEDULE_ADDRESS = 0;

class EEPROM_IO
{
public:
    static void initEEPROM();
    static std::string readSchedule();
    static void appendSchedule(const std::string &s);
    static void overwriteSchedule(const std::string &s);
    static void deleteFromSchedule(const std::string &s);
};

void EEPROM_IO::initEEPROM()
{
    EEPROM.begin(EEPROM_SIZE);

    if (RESET_EEPROM_ON_STARTUP)
        overwriteSchedule("Daily 20:30; Weekly Sunday 18:20;");

    OLED::write(INITIATED_EEPROM);
};

std::string EEPROM_IO::readSchedule()
{
    char buffer[EEPROM_SIZE];

    for (size_t i = 0; i < EEPROM_SIZE; i++)
    {
        buffer[i] = EEPROM.read(SCHEDULE_ADDRESS + i);
        if (buffer[i] == '\0')
        {
            break;
        }
    }

    return buffer;
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
};

void EEPROM_IO::appendSchedule(const std::string &s)
{
    std::string current = readSchedule();
    std::string formatted = S_UTILS::removeNullTerminators(current).c_str();
    overwriteSchedule(formatted + s.c_str());
};

void EEPROM_IO::deleteFromSchedule(const std::string &s)
{
    std::string current = readSchedule();
    std::string formatted = S_UTILS::removeNullTerminators(current).c_str();
    std::string result = S_UTILS::replaceSubstr(formatted, s, "");
    overwriteSchedule(result.c_str());
};

#endif
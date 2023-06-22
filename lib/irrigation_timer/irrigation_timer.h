#pragma once
#ifndef IRRIGATION_TIMER_H
#define IRRIGATION_TIMER_H
#include <Arduino.h>
#include <string>
#include <vector>
#include "s_utils.h"
#include "eeprom_io.h"
#include "clock.h"
#include "physical_io.h"
#include "transmitter.h"

const float VOLTAGE_LIMIT = 3.3;
const int TIME_LIMIT_MIN = 30;

typedef struct irrigation_schedule
{
    std::string type;
    std::string day;
    std::string hour;
    float durationMinutes;

} irrigation_schedule;

typedef struct irrigation
{
    float durationMinutes;
    bool inProgress;
    unsigned long elapsedMs;
    unsigned long startTimeMs;
    float elapsedSeconds;
} irrigation;

irrigation mainIrrigation = {1, false, 0, 0, 0};

class IRRIGATION_TIMER
{
private:
    static int getIrrigationDuration(const float &vPercentage);

public:
    static irrigation_schedule formatIrrigationScheduleStr(const std::string &scheduleStr);
    static void setManualIrrigationDuration(const float &voltage);
    static irrigation getIrrigation();
    static void stopIrrigation();
    static void startIrrigation();
    static void irrigationHandler();
    static void irrigationScheduleHandler();
};

irrigation_schedule IRRIGATION_TIMER::formatIrrigationScheduleStr(const std::string &scheduleStr)
{
    irrigation_schedule is;

    std::string formatted = S_UTILS::removeWhitespace(scheduleStr);
    std::vector<std::string> separated = S_UTILS::splitString(formatted, " ");

    if (S_UTILS::startsWith(scheduleStr, DAILY))
    {
        is.type = DAILY;
        is.durationMinutes = std::stoi(separated[1]);
        is.hour = separated[2];
    }
    else if (S_UTILS::startsWith(scheduleStr, WEEKLY))
    {
        is.type = WEEKLY;
        is.day = separated[1];
        is.durationMinutes = std::stoi(separated[2]);
        is.hour = separated[3];
    }

    return is;
};

int IRRIGATION_TIMER::getIrrigationDuration(const float &vPercentage)
{

    float vPercValue = vPercentage;
    if (vPercValue >= 0 && vPercValue < 10)
    {
        return 1;
    }
    if (vPercValue >= 10 && vPercValue < 20)
    {
        return 5;
    }
    if (vPercValue >= 20 && vPercValue < 35)
    {
        return 10;
    }
    if (vPercValue >= 35 && vPercValue < 50)
    {
        return 15;
    }
    if (vPercValue >= 50 && vPercValue < 75)
    {
        return 20;
    }
    if (vPercValue >= 75 && vPercValue < 90)
    {
        return 25;
    }
    if (vPercValue >= 90 && vPercValue <= 100)
    {
        return 30;
    }

    return 10;
};

void IRRIGATION_TIMER::setManualIrrigationDuration(const float &voltage)
{
    if (mainIrrigation.inProgress)
        return;

    float voltageValue = voltage;
    float percentage = (voltageValue / VOLTAGE_LIMIT) * 100;
    mainIrrigation.durationMinutes = getIrrigationDuration(percentage);
};

irrigation IRRIGATION_TIMER::getIrrigation()
{
    return mainIrrigation;
};

void IRRIGATION_TIMER::stopIrrigation()
{
    mainIrrigation.inProgress = false;
    mainIrrigation.startTimeMs = 0;
    mainIrrigation.elapsedMs = 0;
    mainIrrigation.elapsedSeconds = 0;

    TRANSMITTER::setPowerOff();
    digitalWrite(INTERNAL_LED_PIN, LOW);
};

void IRRIGATION_TIMER::startIrrigation()
{
    mainIrrigation.inProgress = true;
    mainIrrigation.startTimeMs = millis();

    TRANSMITTER::setPowerOn();
    digitalWrite(INTERNAL_LED_PIN, HIGH);
};

void IRRIGATION_TIMER::irrigationHandler()
{
    if (!mainIrrigation.inProgress)
        return;

    mainIrrigation.elapsedMs = millis() - mainIrrigation.startTimeMs;
    mainIrrigation.elapsedSeconds = mainIrrigation.elapsedMs / 1000UL;

    if (mainIrrigation.elapsedSeconds > mainIrrigation.durationMinutes * 60)
    {
        stopIrrigation();
    }
};

void IRRIGATION_TIMER::irrigationScheduleHandler()
{

    if (mainIrrigation.inProgress)
        return;

    char buffer[EEPROM_SIZE];
    EEPROM_IO::readSchedule(buffer);
    std::vector<std::string> separated = S_UTILS::splitString(buffer, SCHEDULE_SEPARATOR);

    for (int i = 0; i < separated.size(); i++)
    {
        irrigation_schedule is = formatIrrigationScheduleStr(S_UTILS::removeWhitespace(separated[i]));

        if (is.type == DAILY)
        {
            if (is.hour == CLOCK::getTime())
            {
                mainIrrigation.durationMinutes = is.durationMinutes;
                startIrrigation();
            }
        }
        else if (is.type == WEEKLY)
        {
            if (is.day == CLOCK::getWeekDayString() && is.hour == CLOCK::getTime())
            {
                mainIrrigation.durationMinutes = is.durationMinutes;
                startIrrigation();
            }
        }
    }
};

#endif
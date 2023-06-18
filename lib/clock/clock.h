
#pragma once
#ifndef CLOCK_H
#define CLOCK_H
#include <string>
#include "time.h"
#include "oled.h"
#include "frequencies.h"
#include "env.h"

#define CLOCK_INITIATED "Initiated CLOCK"

struct tm timeinfo;

class CLOCK
{

public:
    static void initClock();
    static void updateClient();
    static int getWeekDay();
    static int getMonthDay();
    static int getMonth();
    static int getYear();
    static std::string getTime();
    static std::string getDate();
};

void CLOCK::initClock()
{
    configTime(NTP_TZ_OFFSET_S, NTP_DAYLIGHT_OFFSET_S, NTP_POOL);
    getLocalTime(&timeinfo);
    OLED::write(CLOCK_INITIATED);
};

void CLOCK::updateClient()
{
    getLocalTime(&timeinfo);
};

int CLOCK::getWeekDay()
{
    return timeinfo.tm_wday;
};

int CLOCK::getMonthDay()
{
    return timeinfo.tm_mday;
};

int CLOCK::getMonth()
{

    return timeinfo.tm_mon + 1;
};

int CLOCK::getYear()
{
    return timeinfo.tm_year + 1900;
};

std::string CLOCK::getTime()
{
    std::string hour = timeinfo.tm_hour < 10 ? "0" + std::to_string(timeinfo.tm_hour) : std::to_string(timeinfo.tm_hour);
    std::string minutes = timeinfo.tm_min < 10 ? "0" + std::to_string(timeinfo.tm_min) : std::to_string(timeinfo.tm_min);
    return hour + ":" + minutes;
};

std::string CLOCK::getDate()
{
    std::string sep = "/";
    std::string monthDay = std::to_string(getMonthDay());
    std::string month = std::to_string(getMonth());
    std::string year = std::to_string(getYear());
    return monthDay + sep + month + sep + year;
};

#endif
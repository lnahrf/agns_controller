
#pragma once
#ifndef CLOCK_H
#define CLOCK_H
#include <string>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "time.h"
#include "oled.h"
#include "frequencies.h"
#include "env.h"

#define CONNECTING_TO_NETWORK "Connecting to network"
#define NETWORK_CONNECTED "Network connected!"
#define CLOCK_INITIATED "Clock initiated"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_POOL, NTP_TZ_OFFSET_S + NTP_DAYLIGHT_OFFSET_S);
bool connected = false;

class CLOCK
{
public:
    static void initClock();
    static bool getConnectionStatus();
    static void updateClient();
    static int getWeekDay();
    static std::string getWeekDayString();
    static std::string getMonthDay();
    static std::string getMonth();
    static std::string getYear();
    static std::string getTime();
    static std::string getDate();
};

void CLOCK::initClock()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        OLED::write(CONNECTING_TO_NETWORK);
    }

    connected = true;
    OLED::write(NETWORK_CONNECTED);

    timeClient.begin();
    timeClient.update();
    OLED::write(CLOCK_INITIATED);
};

bool CLOCK::getConnectionStatus()
{
    return connected;
};

void CLOCK::updateClient()
{
    timeClient.update();
};

std::string CLOCK::getTime()
{
    return timeClient.getFormattedTime().c_str();
};

std::string CLOCK::getWeekDayString()
{
    int day = timeClient.getDay();
    switch (day)
    {
    case 0:
        return SUNDAY;
    case 1:
        return MONDAY;
    case 2:
        return TUESDAY;
    case 3:
        return WEDNESDAY;
    case 4:
        return THURSDAY;
    case 5:
        return FRIDAY;
    case 6:
        return SATURDAY;
    default:
        return SUNDAY;
    };
};

int CLOCK::getWeekDay()
{
    return timeClient.getDay() + 1;
};

std::string CLOCK::getMonthDay()
{
    time_t time = timeClient.getEpochTime();
    tm *formatted = localtime(&time);
    uint8_t monthDay = formatted->tm_mday;
    return std::to_string(monthDay);
};

std::string CLOCK::getMonth()
{
    time_t time = timeClient.getEpochTime();
    tm *formatted = localtime(&time);
    uint8_t month = formatted->tm_mon + 1;
    return std::to_string(month);
};

std::string CLOCK::getYear()
{
    time_t time = timeClient.getEpochTime();
    tm *formatted = localtime(&time);
    uint8_t year = formatted->tm_year;
    return std::to_string(year + 1900);
};

std::string CLOCK::getDate()
{
    std::string sep = "/";
    return getMonthDay().c_str() + sep + getMonth().c_str() + sep + getYear().c_str();
};

#endif
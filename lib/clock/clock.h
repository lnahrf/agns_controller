
#pragma once
#ifndef CLOCK_H
#define CLOCK_H
#include <string>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "oled.h"
#include "env.h"

#define CONNECTING_TO_NETWORK "Connecting to network..."
#define NETWORK_CONNECTED "Network connected"
#define CLOCK_INITIATED "Clock initiated"

#define NTP_URL "pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_URL);
bool connected = false;

class CLOCK
{
public:
    static void initClock();
    static bool getConnectionStatus();
    static void updateClient();
    static std::string getTime();
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

#endif
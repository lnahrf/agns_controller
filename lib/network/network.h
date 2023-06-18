#pragma once
#ifndef NETWORK_H
#define NETWORK_H
#include <WiFi.h>
#include "env.h"

class NETWORK
{

public:
    static void connect();
    static bool isConnected();
    static void validateConnection();
};

void NETWORK::connect()
{
    WiFi.disconnect();
    WiFi.setSleep(WIFI_PS_NONE);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool NETWORK::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
};

void NETWORK::validateConnection()
{

    if (!isConnected())
    {
        connect();
    }
};

#endif
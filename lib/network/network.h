#pragma once
#ifndef NETWORK_H
#define NETWORK_H
#include <WiFi.h>
#include <string>
#include "env.h"

class NETWORK
{

public:
    static void connect();
    static bool isConnected();
    static void validateConnection();
    static std::string getIp();
};

void NETWORK::connect()
{
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(WIFI_PS_NONE);

    if (isConnected())
        WiFi.disconnect();

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

std::string NETWORK::getIp()
{
    return WiFi.localIP().toString().c_str();
}

#endif
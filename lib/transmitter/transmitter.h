#pragma once
#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include <esp_now.h>
#include <WiFi.h>

#define ESP_INIT_FAILED "Failed to init ESP"
#define ESP_INIT_SUCCESS "ESP initiated"

typedef struct message
{
    bool power;
} message;

uint8_t broadcastAddress[] = {0x94, 0xB5, 0x55, 0xF4, 0x99, 0x44};
message payload;
esp_now_peer_info_t nodeData;

class TRANSMITTER
{
public:
    static bool initESP();
};

bool TRANSMITTER::initESP()
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        return false;
    }
    return true;
};

#endif
// bool connectAGNSNode();
// void powerOn();
// void powerOff();

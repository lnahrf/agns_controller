#pragma once
#ifndef TRANSMITTER_H
#define TRANSMITTER_H
#include <esp_now.h>
#include <WiFi.h>
#include <string>
#include "oled.h"
#include "mac_utils.h"
#include "env.h"

#define ESP_INIT_FAILED "Failed to init ESP"
#define ESP_INIT_SUCCESS "Initiated ESP"
#define ESP_AGNS_CONFIG_FAILED "Failed to configure AGNS node"
#define ESP_AGNS_CONFIG_SUCCESS "Node registered"
#define ESP_AGNS_NODES_CONNECTED "AGNS nodes registered: "
#define ESP_SIGNAL_SENT "AGNS sent signal successfuly"
#define ESP_SIGNAL_FAILED "AGNS failed to send signal"

typedef struct state
{
    bool power;
} state;

struct node
{
    uint8_t broadcastAddress[6];
    esp_now_peer_info_t data;
};

node nodes[NUM_OF_AGNS_NODES];
size_t nodesConnected = 0;

state outgoingState;

class TRANSMITTER
{
private:
    int powerState;

public:
    static std::string printNodesRegistered();
    static void initESP();
    static void configAGNSNodes();
    static void setPowerOn();
    static void setPowerOff();
    static void sendSignal();
};

void TRANSMITTER::initESP()
{
    if (esp_now_init() != ESP_OK)
    {
        OLED::write(ESP_INIT_FAILED);
        for (;;)
            ;
    }
    OLED::write(ESP_INIT_SUCCESS);
};

void TRANSMITTER::configAGNSNodes()
{

    std::vector<std::string> macStrings = S_UTILS::splitString(AGNS_NODES, ",");
    std::string space = std::string(" ");
    for (int i = 0; i < NUM_OF_AGNS_NODES; i++)
    {
        MAC_UTILS::stringToMac(macStrings.at(i), nodes[i].broadcastAddress);

        memcpy(nodes[i].data.peer_addr, nodes[i].broadcastAddress, 6);
        nodes[i].data.channel = 0;
        nodes[i].data.encrypt = false;

        if (esp_now_add_peer(&nodes[i].data) != ESP_OK)
        {
            OLED::write(ESP_AGNS_CONFIG_FAILED + space + macStrings.at(i));
            for (;;)
                ;
        }

        OLED::write(ESP_AGNS_CONFIG_SUCCESS + space + macStrings.at(i));
    }
    nodesConnected = sizeof(nodes) / sizeof(nodes[0]);
};

void TRANSMITTER::setPowerOn()
{
    outgoingState.power = true;
};

void TRANSMITTER::setPowerOff()
{
    outgoingState.power = false;
};

void TRANSMITTER::sendSignal()
{
    for (int i = 0; i < NUM_OF_AGNS_NODES; i++)
    {
        esp_err_t result = esp_now_send(nodes[i].broadcastAddress, (uint8_t *)&outgoingState, sizeof(outgoingState));
        if (result != ESP_OK)
        {
            Serial.println(ESP_SIGNAL_FAILED);
        }
        else
        {
            Serial.println(ESP_SIGNAL_SENT);
        }
    }
};

std::string TRANSMITTER::printNodesRegistered()
{
    std::stringstream ss;
    ss << nodesConnected;
    return ss.str();
}

#endif

#pragma once
#ifndef WEBGUI_H
#define WEBGUI_H
#include <WiFi.h>
#include <WebServer.h>
#include <string>
#include "oled.h"
#include "env.h"

#define CONNECTING_TO_NETWORK_WEBGUI "Connecting WEBGUI"
#define NETWORK_CONNECTED_WEBGUI "Connected WEBGUI!"
#define WEBGUI_INIT_SUCCESS "Initiated WEBGUI"

WebServer server(80);

class WEBGUI
{
private:
    static void rootRouter();

public:
    static void initServer();
    static std::string getIp();
    static void handleRequests();
};

void WEBGUI::rootRouter()
{
    server.send(200, "text/html", "<h1>Hello world!</h1>");
};

void WEBGUI::initServer()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        OLED::write(CONNECTING_TO_NETWORK_WEBGUI);
    }

    OLED::write(NETWORK_CONNECTED_WEBGUI);

    server.on("/", rootRouter);
    server.begin();
    OLED::write(WEBGUI_INIT_SUCCESS);
}

std::string WEBGUI::getIp()
{
    return WiFi.localIP().toString().c_str();
}

void WEBGUI::handleRequests()
{
    return server.handleClient();
};

#endif
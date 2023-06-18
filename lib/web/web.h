#pragma once
#ifndef WEB_H
#define WEB_H
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <string>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "oled.h"
#include "eeprom_io.h"
#include "env.h"

#define WEB_INIT_SUCCESS "Initiated WEB"

AsyncWebServer server(80);

class WEB
{
private:
    static void notFound(AsyncWebServerRequest *request);
    static void rootRouter(AsyncWebServerRequest *request);
    static void styleRouter(AsyncWebServerRequest *request);
    static void assetsRouter(AsyncWebServerRequest *request);
    static void scheduleGETRouter(AsyncWebServerRequest *request);
    static void schedulePOSTRouter(AsyncWebServerRequest *request);

public:
    static void initServer();
};

void WEB::notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
    return request->client()->close();
};

void WEB::rootRouter(AsyncWebServerRequest *request)
{
    request->send(SPIFFS, "/index.html", "text/html");
    return request->client()->close();
};

void WEB::styleRouter(AsyncWebServerRequest *request)
{
    String url = request->url();

    if (url.equals("/styles/style.css"))
    {
        request->send(SPIFFS, "/styles/style.css", "text/css");
    }

    return request->client()->close();
};

void WEB::assetsRouter(AsyncWebServerRequest *request)
{
    String url = request->url();

    if (url.equals("/assets/favicon.ico"))
    {
        request->send(SPIFFS, "/assets/favicon.ico", "image/x-icon");
    }
    else if (url.equals("/assets/agns_logo.png"))
    {
        request->send(SPIFFS, "/assets/agns_logo.png", "image/png");
    }
    return request->client()->close();
};

void WEB::scheduleGETRouter(AsyncWebServerRequest *request)
{
    Serial.println("In get schedule");
    std::string schedule = EEPROM_IO::readSchedule();

    DynamicJsonDocument doc(1024);
    doc["schedule"] = schedule.length() > 0 ? schedule : "";

    String jsonStringified;
    serializeJson(doc, jsonStringified);

    request->send(200, "application/json", jsonStringified);

    return request->client()->close();
};

void WEB::schedulePOSTRouter(AsyncWebServerRequest *request){
    // add to Schedule
    // remove from schedule

};

void WEB::initServer()
{
    server.on("/", HTTP_GET, rootRouter);
    server.on("/styles/*", HTTP_GET, styleRouter);
    server.on("/assets/*", HTTP_GET, assetsRouter);
    server.on("/schedule", HTTP_GET, scheduleGETRouter);
    server.begin();
    OLED::write(WEB_INIT_SUCCESS);
}

#endif
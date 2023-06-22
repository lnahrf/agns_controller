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
    static void modulesRouter(AsyncWebServerRequest *request);
    static void scheduleGETRouter(AsyncWebServerRequest *request);
    static void schedulePOSTRouter(AsyncWebServerRequest *request);
    static void scheduleDELETERouter(AsyncWebServerRequest *request);

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

    if (url.equals("/assets/agns_logo.png"))
    {
        request->send(SPIFFS, "/assets/agns_logo.png", "image/png");
    }

    return request->client()->close();
};

void WEB::modulesRouter(AsyncWebServerRequest *request)
{
    String url = request->url();

    if (url.equals("/modules/index.js"))
    {
        request->send(SPIFFS, "/modules/index.js", "text/javascript");
    }
    else if (url.equals("/modules/schedule.js"))
    {
        request->send(SPIFFS, "/modules/schedule.js", "text/javascript");
    }
    else if (url.equals("/modules/form.js"))
    {
        request->send(SPIFFS, "/modules/form.js", "text/javascript");
    }
    else if (url.equals("/modules/format.js"))
    {
        request->send(SPIFFS, "/modules/format.js", "text/javascript");
    }
    else if (url.equals("/modules/service.js"))
    {
        request->send(SPIFFS, "/modules/service.js", "text/javascript");
    }
    else if (url.equals("/modules/handlers.js"))
    {
        request->send(SPIFFS, "/modules/handlers.js", "text/javascript");
    }
    else if (url.equals("/modules/enums.js"))
    {
        request->send(SPIFFS, "/modules/enums.js", "text/javascript");
    }

    return request->client()->close();
};

void WEB::scheduleGETRouter(AsyncWebServerRequest *request)
{
    char buffer[EEPROM_SIZE];
    EEPROM_IO::readSchedule(buffer);

    size_t charCount = sizeof(buffer) / sizeof(char);
    DynamicJsonDocument doc(512);
    doc["schedule"] = charCount > 0 ? buffer : "";

    String jsonStringified;
    serializeJson(doc, jsonStringified);

    request->send(200, "application/json", jsonStringified);
    return request->client()->close();
};

void WEB::schedulePOSTRouter(AsyncWebServerRequest *request)
{

    if (!request->hasParam("schedule"))
    {
        request->send(400, "text/plain", "Bad Request Habibi");
        return request->client()->close();
    }

    String schedule = request->getParam("schedule")->value();
    EEPROM_IO::appendSchedule(schedule.c_str());

    request->send(200);
    return request->client()->close();
};

void WEB::scheduleDELETERouter(AsyncWebServerRequest *request)
{
    if (!request->hasParam("schedule"))
    {
        request->send(400, "text/plain", "Bad Request Habibi");
        return request->client()->close();
    }

    String schedule = request->getParam("schedule")->value();
    EEPROM_IO::deleteFromSchedule(schedule.c_str());

    request->send(200);
    return request->client()->close();
};

void WEB::initServer()
{
    server.on("/", HTTP_GET, rootRouter);
    server.on("/styles/*", HTTP_GET, styleRouter);
    server.on("/assets/*", HTTP_GET, assetsRouter);
    server.on("/modules/*", HTTP_GET, modulesRouter);
    server.on("/schedule", HTTP_GET, scheduleGETRouter);
    server.on("/schedule", HTTP_POST, schedulePOSTRouter);
    server.on("/schedule", HTTP_DELETE, scheduleDELETERouter);
    server.begin();
    OLED::write(WEB_INIT_SUCCESS);
}

#endif

#pragma once
#ifndef OLED_H
#define OLED_H
#include <string>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TEXT_SIZE 1
#define TEXT_COLOR WHITE
#define OLED_RESET -1

#define OLED_ALLOC_SUCCESS "SSD1306 allocation successful"
#define OLED_ALLOC_FAILURE "SSD1306 allocation failed"
#define OLED_INITIATED "Initiated OLED"

Adafruit_SSD1306 instance(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class OLED
{
public:
    static bool initOLED();
    static void write(const std::string &s);
    static void renderMainScreen(const std::string &nodesConnected, const std::string &time, const std::string &date, const std::string &ip);
};

bool OLED::initOLED()
{
    bool init = instance.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    if (!init)
    {
        Serial.println(OLED_ALLOC_FAILURE);
        return false;
    }
    Serial.println(OLED_ALLOC_SUCCESS);

    instance.clearDisplay();
    instance.setTextSize(TEXT_SIZE);
    instance.setTextColor(TEXT_COLOR);
    instance.setCursor(0, 0);
    instance.println(OLED_INITIATED);
    instance.display();
    delay(1000);
    return true;
}

void OLED::write(const std::string &s)
{
    instance.clearDisplay();
    instance.setCursor(0, 0);
    instance.println(s.c_str());
    instance.display();
    Serial.println(s.c_str());
    delay(600);
}

void OLED::renderMainScreen(const std::string &nodesConnected, const std::string &time, const std::string &date, const std::string &ip)
{
    instance.clearDisplay();
    instance.setTextSize(1);
    instance.setTextColor(WHITE);

    instance.setCursor(0, 0);
    instance.println("AGNS");
    instance.setCursor(30, 0);
    instance.println(time.substr(0, 5).c_str());
    instance.setCursor(65, 0);
    instance.println(date.c_str());

    instance.setCursor(0, 10);
    instance.println("IP: ");
    instance.setCursor(20, 10);
    instance.println(ip.c_str());

    instance.setCursor(0, 20);
    instance.println("Nodes registered:");
    instance.setTextSize(3);
    instance.setCursor(0, 40);
    instance.println(nodesConnected.c_str());
    instance.display();
}

#endif
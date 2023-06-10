
#pragma once
#ifndef OLED_H
#define OLED_H
#include <string>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TEXT_SIZE 1.4
#define TEXT_COLOR WHITE
#define OLED_RESET -1

#define OLED_ALLOC_SUCCESS "SSD1306 allocation successful"
#define OLED_ALLOC_FAILURE "SSD1306 allocation failed"
#define OLED_INITIATED "OLED initiated"

Adafruit_SSD1306 instance(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class OLED
{
public:
    static bool initOLED();
    static void write(const std::string &s);
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
    return true;
}

void OLED::write(const std::string &s)
{
    instance.clearDisplay();
    instance.setCursor(0, 0);
    instance.println(s.c_str());
    instance.display();
}

#endif
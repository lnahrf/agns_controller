
#pragma once
#ifndef OLED_H
#define OLED_H
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>
#include <vector>
#include "s_utils.h"
#include "frequencies.h"
#include "env.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TEXT_SIZE 1
#define TEXT_COLOR WHITE
#define OLED_RESET -1

#define OLED_ALLOC_SUCCESS "SSD1306 allocation successful"
#define OLED_ALLOC_FAILURE "SSD1306 allocation failed"
#define OLED_INITIATED "Initiated OLED"

const int MAIN_SCREEN = 0;
const int SCHEDULE_SCREEN = 1;
const int IRRIGATION_SCREEN = 2;
const int SCREEN_COUNT = 3;

int currentScreen = 0;

Adafruit_SSD1306 instance(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class OLED
{

private:
public:
    static bool initOLED();
    static int getCurrentScreen();
    static void write(const std::string &s);
    static void nextScreen();
    static void renderMainScreen(const std::string &nodesConnected, const std::string &time, const std::string &date, const std::string &ip);
    static void renderScheduleScreen(char *schedule);
    static void renderIrrigationScreen(const bool &inProgress, const int &durationMinutes, const float &elapsedSeconds);
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

    instance.setCursor(0, 12);
    instance.println("IP: ");
    instance.setCursor(20, 12);
    instance.println(ip.c_str());

    instance.setTextSize(3);
    instance.setCursor(0, 36);
    instance.println(nodesConnected.c_str());
    instance.display();
}

int OLED::getCurrentScreen()
{
    return currentScreen;
};

void OLED::nextScreen()
{
    if (currentScreen == SCREEN_COUNT - 1)
    {
        currentScreen = 0;
    }
    else
    {
        currentScreen++;
    }
}

void OLED::renderScheduleScreen(char *schedule)
{

    instance.clearDisplay();
    instance.setTextSize(1);
    instance.setTextColor(WHITE);
    instance.setCursor(0, 0);
    instance.println("SCHEDULE");
    instance.drawLine(0, 10, SCREEN_WIDTH, 10, WHITE);
    int y = 15;

    std::vector<std::string> separated = S_UTILS::splitString(schedule, SCHEDULE_SEPARATOR);

    for (int i = 0; i < separated.size(); i++)
    {
        std::string current = S_UTILS::removeWhitespace(separated[i]);
        instance.setCursor(0, y);
        instance.println(current.c_str());
        y += 12;
    }

    instance.display();
}

void OLED::renderIrrigationScreen(const bool &inProgress, const int &durationMinutes, const float &elapsedSeconds)
{
    instance.clearDisplay();
    instance.setTextSize(1);
    instance.setTextColor(WHITE);
    instance.setCursor(0, 0);
    instance.println("IRRIGATION");
    instance.drawLine(0, 10, SCREEN_WIDTH, 10, WHITE);

    instance.setCursor(0, 15);
    std::string durationText = "DURATION: " + std::to_string(durationMinutes) + " MINUTES";
    instance.println(durationText.c_str());

    instance.setCursor(0, 27);
    instance.println(elapsedSeconds);

    instance.display();
};

#endif
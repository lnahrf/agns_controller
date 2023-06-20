#pragma once
#ifndef PHYSICAL_IO_H
#define PHYSICAL_IO_H
#include <Arduino.h>

const int BUTTON_COUNT = 2;
const int POTENTIOMETER_COUNT = 1;
const int LONG_PRESS_THRESHOLD_MS = 2000;

struct button_io
{
    int pin;
    int lastState;
    unsigned long startTime;
    unsigned long elapsedTime;
    bool pressed;
    bool previousLongPress;
};

struct potentiometer_io
{
    int pin;
    float voltage;
};

button_io buttons[BUTTON_COUNT];
potentiometer_io potentiometers[POTENTIOMETER_COUNT];

button_io MENU_BUTTON = {5, HIGH, 0, 0, false, false};
potentiometer_io MANUAL_IRRIGATION_POT = {34, 0};

class PHYSICAL_IO
{
private:
    static float floatMap(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

public:
    static void populateIO();
    static button_io *getButtonByPin(const int &PIN);
    static potentiometer_io *getPotentiometerByPin(const int &PIN);
    static void initIO(const int &PIN);
    static void handleButtonIO(
        const int &PIN, const std::function<void()> &handler, const std::function<void()> &longPressHandler);
    static void handlePotentiometerIO(const int &PIN, const std::function<void(float)> &handler);
};

void PHYSICAL_IO::populateIO()
{
    buttons[0] = MENU_BUTTON;
    potentiometers[0] = MANUAL_IRRIGATION_POT;
};

button_io *PHYSICAL_IO::getButtonByPin(const int &PIN)
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        if (PIN == buttons[i].pin)
            return &buttons[i];
    }
    return nullptr;
};

potentiometer_io *PHYSICAL_IO::getPotentiometerByPin(const int &PIN)
{
    for (int i = 0; i < POTENTIOMETER_COUNT; i++)
    {
        if (PIN == potentiometers[i].pin)
            return &potentiometers[i];
    }
    return nullptr;
};

void PHYSICAL_IO::initIO(const int &PIN)
{
    int pinValue = PIN;
    pinMode(pinValue, INPUT);
};

void PHYSICAL_IO::handleButtonIO(
    const int &PIN, const std::function<void()> &handler, const std::function<void()> &longPressHandler = []() {})
{
    int pinValue = PIN;
    int currentState = digitalRead(pinValue);
    button_io *button = getButtonByPin(PIN);

    if (button == nullptr)
        return;

    if (currentState == LOW && !button->pressed)
    {
        // Push down
        button->pressed = true;
        button->startTime = millis();
        button->elapsedTime = 0;
    }
    else if (currentState == HIGH && button->pressed)
    {
        // Release
        if (button->elapsedTime < LONG_PRESS_THRESHOLD_MS && !button->previousLongPress)
        {
            // Short press functionality
            handler();
            delay(100);
        }

        // Reset after short press
        button->pressed = false;
        button->startTime = 0;
        button->elapsedTime = 0;
        button->previousLongPress = false;
    }

    if (button->pressed && !button->previousLongPress)
    {
        button->elapsedTime = millis() - button->startTime;
        if (button->elapsedTime >= LONG_PRESS_THRESHOLD_MS)
        {
            // Long press functionality
            longPressHandler();

            // Reset after long press
            button->startTime = 0;
            button->elapsedTime = 0;
            button->pressed = false;
            button->previousLongPress = true;

            delay(100);
        }
    }
};

void PHYSICAL_IO::handlePotentiometerIO(const int &PIN, const std::function<void(float)> &handler)
{
    int pinValue = PIN;
    int analogValue = analogRead(pinValue);
    float voltage = floatMap(analogValue, 0, 4095, 0, 3.3);
    handler(voltage);
};

#endif
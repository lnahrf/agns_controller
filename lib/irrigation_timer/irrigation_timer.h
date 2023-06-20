#pragma once
#ifndef IRRIGATION_TIMER_H
#define IRRIGATION_TIMER_H
#include <Arduino.h>
#include <string>

const float VOLTAGE_LIMIT = 3.3;
const int TIME_LIMIT_MIN = 30;

typedef struct irrigation
{
    float durationMinutes;
    bool inProgress;
    unsigned long elapsedMs;
    unsigned long startTimeMs;
    float elapsedSeconds;
} irrigation;

irrigation mainIrrigation = {1, false, 0, 0, 0};

class IRRIGATION_TIMER
{
private:
    static int getIrrigationDuration(const float &vPercentage);

public:
    static void setManualIrrigationDuration(const float &voltage);
    static irrigation getIrrigation();
    static void stopIrrigation();
    static void startIrrigation();
    static void irrigationHandler();
};

int IRRIGATION_TIMER::getIrrigationDuration(const float &vPercentage)
{

    float vPercValue = vPercentage;
    if (vPercValue >= 0 && vPercValue < 10)
    {
        return 1;
    }
    if (vPercValue >= 10 && vPercValue < 20)
    {
        return 5;
    }
    if (vPercValue >= 20 && vPercValue < 35)
    {
        return 10;
    }
    if (vPercValue >= 35 && vPercValue < 50)
    {
        return 15;
    }
    if (vPercValue >= 50 && vPercValue < 75)
    {
        return 20;
    }
    if (vPercValue >= 75 && vPercValue < 90)
    {
        return 25;
    }
    if (vPercValue >= 90 && vPercValue <= 100)
    {
        return 30;
    }

    return 10;
};

void IRRIGATION_TIMER::setManualIrrigationDuration(const float &voltage)
{
    if (mainIrrigation.inProgress)
        return;

    float voltageValue = voltage;
    float percentage = (voltageValue / VOLTAGE_LIMIT) * 100;
    mainIrrigation.durationMinutes = getIrrigationDuration(percentage);
};

irrigation IRRIGATION_TIMER::getIrrigation()
{
    return mainIrrigation;
};

void IRRIGATION_TIMER::stopIrrigation()
{
    Serial.println("Stopping irrigation");
    mainIrrigation.inProgress = false;
    mainIrrigation.startTimeMs = 0;
    mainIrrigation.elapsedMs = 0;
    mainIrrigation.elapsedSeconds = 0;
};

void IRRIGATION_TIMER::startIrrigation()
{
    mainIrrigation.inProgress = true;
    mainIrrigation.startTimeMs = millis();
};

void IRRIGATION_TIMER::irrigationHandler()
{
    if (!mainIrrigation.inProgress)
        return;

    mainIrrigation.elapsedMs = millis() - mainIrrigation.startTimeMs;
    mainIrrigation.elapsedSeconds = mainIrrigation.elapsedMs / 1000UL;

    if (mainIrrigation.elapsedSeconds > mainIrrigation.durationMinutes * 60)
    {
        stopIrrigation();
    }
}

#endif
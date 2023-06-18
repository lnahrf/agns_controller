#pragma once
#ifndef IRRIGATION_TIMER_H
#define IRRIGATION_TIMER_H
#include <string>

typedef struct irrigation_schedule
{
    std::string frequency;
    std::string time;
    int duration;
} irrigation_schedule;

class IRRIGATION_TIMER
{
public:
    static std::string scheduleToString(const irrigation_schedule &is);
    static void getSchedule();
    static void addSchedule();
    static void deleteSchedule();
};

#endif
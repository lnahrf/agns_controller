#pragma once
#ifndef MAC_UTILS_H
#define MAC_UTILS_H
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "s_utils.h"

class MAC_UTILS
{
public:
    static void stringToMac(const std::string &mStr, uint8_t *mArr);
};

void MAC_UTILS::stringToMac(const std::string &mStr, uint8_t *mArr)
{
    std::istringstream iss(mStr);
    unsigned int octet;

    for (int i = 0; i < 6; i++)
    {
        iss >> std::hex >> octet;
        mArr[i] = static_cast<uint8_t>(octet);
        iss.ignore(1, ':');
    }
}

#endif
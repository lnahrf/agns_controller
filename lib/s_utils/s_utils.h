#pragma once
#ifndef S_UTILS_H
#define S_UTILS_H
#include <string>
#include <vector>
#include <regex>

class S_UTILS
{
public:
    static std::vector<std::string> splitString(const std::string &s, const std::string &r);
};

std::vector<std::string> S_UTILS::splitString(const std::string &s, const std::string &r)
{
    std::regex re(r);
    std::sregex_token_iterator
        first{s.begin(), s.end(), re, -1},
        last;
    return {first, last};
}

#endif
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
    static std::string replaceSubstr(const std::string &original, const std::string &target, const std::string &replacement);
    static std::string removeNullTerminators(const std::string &s);
};

std::vector<std::string> S_UTILS::splitString(const std::string &s, const std::string &r)
{
    std::regex re(r);
    std::sregex_token_iterator
        first{s.begin(), s.end(), re, -1},
        last;
    return {first, last};
}

std::string S_UTILS::replaceSubstr(const std::string &original, const std::string &target, const std::string &replacement)
{
    std::size_t startPos = original.find(target);
    std::string result = "";
    if (startPos != std::string::npos)
    {
        result = original.substr(0, startPos) + replacement + original.substr(startPos + target.length());
    }

    return result;
};

std::string S_UTILS::removeNullTerminators(const std::string &s)
{
    std::string result = s.c_str();
    result.erase(std::remove_if(result.begin(), result.end(), [](char c)
                                { return c == '\0'; }),
                 result.end());
    return result;
};

#endif
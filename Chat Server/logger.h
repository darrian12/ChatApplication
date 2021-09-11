#pragma once

#include <string>

class logger
{
public:
    logger();
    ~logger();

    static void PrintMessage(const std::string& msg);
    static void PrintWarning(const std::string& msg);
    static void PrintError(const std::string& msg);
};

#define LOG(x) logger::PrintMessage(x);
#define LOG_WARNING(x) logger::PrintWarning(x);
#define LOG_ERROR(x) logger::PrintError(x);
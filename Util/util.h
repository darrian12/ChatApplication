#pragma once

#include <string>

namespace Util
{
    class logger
    {
    public:
        logger();
        ~logger();

        static void PrintMessage(const std::string& msg);
        static void PrintWarning(const std::string& msg);
        static void PrintError(const std::string& msg);
    };
}

#define LOG(x) Util::logger::PrintMessage(x);
#define LOG_WARNING(x) Util::logger::PrintWarning(x);
#define LOG_ERROR(x) Util::logger::PrintError(x);
#include "pch.h"
#include "framework.h"

#include "util.h"

#include <iostream>

namespace Util
{
    logger::logger()
    {
    }

    logger::~logger()
    {
    }

    void logger::PrintMessage(const std::string& msg)
    {
        std::cout << "[LOG] " << msg << "\n";
    }

    void logger::PrintWarning(const std::string& msg)
    {
        PrintMessage("WARNING: " + msg);
    }

    void logger::PrintError(const std::string& msg)
    {
        PrintMessage("ERROR: " + msg);
    }
}
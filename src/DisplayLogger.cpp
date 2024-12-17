#include "DisplayLogger.hpp"
#include "Display.hpp"
#include <cstdarg>
#include <stdio.h>

using BB_Display::getDisplay;
using BB_Display::Display;

namespace BB_Logger
{
    std::shared_ptr<DisplayLogger> displayLogger = nullptr;

    std::shared_ptr<DisplayLogger> getDisplayLogger()
    {
        if (displayLogger == nullptr)
        {
            displayLogger = std::make_shared<DisplayLogger>();
        }
        return displayLogger;
    }

    DisplayLogger::DisplayLogger()
    {
        this->display = getDisplay();
    }

    void DisplayLogger::logf(LogLevel level, const char *message, ...)
    {
        va_list args;
        va_start(args, message);
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), message, args);
        va_end(args);

        log(level, buffer);
    }

    void DisplayLogger::log(LogLevel level, const char *message)
    {
        auto display = this->display.lock();
        if (!display)
        {
            return;
        }
        
        switch (level)
        {
        case LogLevel::DEBUG:
            display->printf("[DEBUG] %s\n", message);
            break;
        case LogLevel::INFO:
            display->printf("[INFO] %s\n", message);
            break;
        case LogLevel::WARN:
            display->printf("[WARN] %s\n", message);
            break;
        case LogLevel::ERROR:
            display->printf("[ERROR] %s\n", message);
            break;
        case LogLevel::FATAL:
            display->printf("[FATAL] %s\n", message);
            break;
        default:
            break;
        }
    }
}
#include "SerialLogger.hpp"

#include <Arduino.h>

namespace BB_Logger
{   
    std::shared_ptr<SerialLogger> SerialLogger::create()
    {
        return std::make_shared<SerialLogger>();
    }

    SerialLogger::SerialLogger()
    {
        connection = BB_Serial::getSerialConnection();
    }

    void SerialLogger::logf(LogLevel level, const char *message, ...)
    {
        va_list args;
        va_start(args, message);
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), message, args);
        va_end(args);

        log(level, buffer);
    }

    void SerialLogger::log(LogLevel level, const char *message)
    {
        auto serial = connection->getSerial();
        switch (level)
        {
        case LogLevel::DEBUG:
            serial->print("D: ");
            break;
        case LogLevel::INFO:
            serial->print("I: ");
            break;
        case LogLevel::WARN:
            serial->print("W: ");
            break;
        case LogLevel::ERROR:
            serial->print("E: ");
            break;
        case LogLevel::FATAL:
            serial->print("F: ");
            break;
        default:
            break;
        }
        serial->println(message);
    }
}
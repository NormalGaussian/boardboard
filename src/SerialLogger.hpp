#pragma once

#include "SerialConnection.hpp"
#include "Logger.hpp"
#include <memory>

using BB_Serial::SerialConnection;

namespace BB_Logger {
    class SerialLogger : public Logger {
    private:
        std::shared_ptr<SerialConnection> connection;
        void log(LogLevel level, const char *message) override;
    public:
        SerialLogger();
        static std::shared_ptr<SerialLogger> create();
    };
};

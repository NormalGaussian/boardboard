#include "Logger.hpp"
#include <memory>
#include <cstdarg>
#include <cstring>

namespace BB_Logger {
    char buffer[256];

    void Logger::logfva(LogLevel level, const char *message, va_list args) {
        memset(buffer, 0, sizeof(buffer));
        vsnprintf(buffer, sizeof(buffer), message, args);
        log(level, buffer);
    }
    void Logger::logf(LogLevel level, const char *message, ...) {
        va_list args;
        va_start(args, message);
        logfva(level, message, args);
        va_end(args);
    }

    void LoggerMock::log(LogLevel level, const char *message) {}
    void LoggerMock::logf(LogLevel level, const char *message, ...) {}
    void LoggerMock::logfva(LogLevel level, const char *message, va_list args) {}

    std::shared_ptr<LoggerMock> loggerMock = std::make_shared<LoggerMock>();
    std::shared_ptr<Logger> defaultLogger = loggerMock;
    std::shared_ptr<Logger> setDefaultLogger(std::shared_ptr<Logger> logger) {
        return defaultLogger = logger;
    }
};

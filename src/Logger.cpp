#include "Logger.hpp"

namespace BB_Logger {
    void LoggerMock::log(LogLevel level, const char *message) {}
    void LoggerMock::logf(LogLevel level, const char *message, ...) {}

    std::shared_ptr<LoggerMock> loggerMock = std::make_shared<LoggerMock>();
    std::shared_ptr<Logger> defaultLogger = loggerMock;
    std::shared_ptr<Logger> setDefaultLogger(std::shared_ptr<Logger> logger) {
        return defaultLogger = logger;
    }
};

#pragma once

#include <memory>
#include <cstdarg>

namespace BB_Logger
{
    enum LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    class Logger {
    private:
        LogLevel defaultLevel = LogLevel::INFO;
        LogLevel minLevel = LogLevel::DEBUG;
        virtual void log(LogLevel level, const char *message) = 0;
        void logf(LogLevel level, const char *message, ...);
        void logfva(LogLevel level, const char *message, va_list args);
    public:
        void setDefaultLevel(LogLevel level) {
            defaultLevel = level;
        };
        void setMinLevel(LogLevel level) {
            minLevel = level;
        };
        void log(const char *message) {
            log(defaultLevel, message);
        };
        void logf(const char *message, ...) {
            va_list args;
            va_start(args, message);
            logfva(defaultLevel, message, args);
            va_end(args);
        };
        void debug(const char *message) {
            log(LogLevel::DEBUG, message);
        };
        void debugf(const char *message, ...) {
            va_list args;
            va_start(args, message);
            logfva(LogLevel::DEBUG, message, args);
            va_end(args);
        };
        void info(const char *message) {
            log(LogLevel::INFO, message);
        };
        void infof(const char *message, ...) {
            va_list args;
            va_start(args, message);
            logfva(LogLevel::INFO, message, args);
            va_end(args);
        };
        void warn(const char *message) {
            log(LogLevel::WARN, message);
        };
        void warnf(const char *message, ...) {
            va_list args;
            va_start(args, message);
            logfva(LogLevel::WARN, message, args);
            va_end(args);
        };
        void error(const char *message) {
            log(LogLevel::ERROR, message);
        };
        void errorf(const char *message, ...) {
            va_list args;
            va_start(args, message);
            logfva(LogLevel::ERROR, message, args);
            va_end(args);
        };
        void fatal(const char *message) {
            log(LogLevel::FATAL, message);
        };
        void fatalf(const char *message, ...) {
            va_list args;
            va_start(args, message);
            logfva(LogLevel::FATAL, message, args);
            va_end(args);
        };
    };

    class LoggerMock : public Logger {
        private:
            void log(LogLevel level, const char *message) override;
            void logf(LogLevel level, const char *message, ...);
            void logfva(LogLevel level, const char *message, va_list args);
    };

    extern std::shared_ptr<LoggerMock> loggerMock;
    extern std::shared_ptr<Logger> defaultLogger;

    std::shared_ptr<Logger> setDefaultLogger(std::shared_ptr<Logger> logger);
}

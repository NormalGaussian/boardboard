#ifndef LOGGER_H
#define LOGGER_H

#include <memory>

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
        virtual void logf(LogLevel level, const char *message, ...) = 0;
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
            logf(defaultLevel, message);
        };
        void debug(const char *message) {
            log(LogLevel::DEBUG, message);
        };
        void debugf(const char *message, ...) {
            logf(LogLevel::DEBUG, message);
        };
        void info(const char *message) {
            log(LogLevel::INFO, message);
        };
        void infof(const char *message, ...) {
            logf(LogLevel::INFO, message);
        };
        void warn(const char *message) {
            log(LogLevel::WARN, message);
        };
        void warnf(const char *message, ...) {
            logf(LogLevel::WARN, message);
        };
        void error(const char *message) {
            log(LogLevel::ERROR, message);
        };
        void errorf(const char *message, ...) {
            logf(LogLevel::ERROR, message);
        };
        void fatal(const char *message) {
            log(LogLevel::FATAL, message);
        };
        void fatalf(const char *message, ...) {
            logf(LogLevel::FATAL, message);
        };
    };

    class LoggerMock : public Logger {
        private:
            void log(LogLevel level, const char *message) override;
            void logf(LogLevel level, const char *message, ...) override;
    };

    extern std::shared_ptr<LoggerMock> loggerMock;
    extern std::shared_ptr<Logger> defaultLogger;

    std::shared_ptr<Logger> setDefaultLogger(std::shared_ptr<Logger> logger);
}

#endif

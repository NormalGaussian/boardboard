#ifndef DISPLAYLOGGER_H
#define DISPLAYLOGGER_H

#include "Logger.hpp"
#include "Display.hpp"
#include <memory>

using BB_Display::Display;

namespace BB_Logger {
    
        class DisplayLogger : public Logger {
        private:
            std::weak_ptr<Display> display;
        public:
            DisplayLogger();
            void log(LogLevel level, const char *message) override;
            void logf(LogLevel level, const char *message, ...) override;
        };

        std::shared_ptr<DisplayLogger> getDisplayLogger();
};

#endif
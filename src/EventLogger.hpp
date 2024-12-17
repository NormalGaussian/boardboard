#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include "Event.hpp"
#include "Logger.hpp"

using BB_Logger::Logger;

namespace BB_Event {
    class EventLogger {
        private:
            void onEvent(Event event);
            std::weak_ptr<Logger> logger;
            std::shared_ptr<Registration> registration;
        public:
            ~EventLogger();
            EventLogger(std::weak_ptr<Logger> logger);
            static std::shared_ptr<EventLogger> create(std::weak_ptr<Logger> logger);
    };
}

#endif
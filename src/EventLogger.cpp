#include "EventLogger.hpp"

using BB_Logger::LogLevel;

namespace BB_Event {

    EventLogger::EventLogger(std::weak_ptr<Logger> logger) {
        this->logger = logger;
    }

    EventLogger::~EventLogger() {}

    std::shared_ptr<EventLogger> EventLogger::create(std::weak_ptr<Logger> logger) {
        return std::make_shared<EventLogger>(logger);
    }

    void EventLogger::onEvent(BB_Event::Event event) {
        auto logger = this->logger.lock();
        if(!logger) {
            return;
        }

        switch(event) {
            case BB_Event::NETWORK_SETTINGS__LOADED:
                logger->log("Network settings loaded");
                break;
            case BB_Event::NETWORK_SETTINGS__FAILED_LOAD:
                logger->log("Failed to load network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__SAVED:
                logger->log("Network settings saved");
                break;
            case BB_Event::NETWORK_SETTINGS__FAILED_SAVE:
                logger->log("Failed to save network settings");
                break;

            case BB_Event::NETWORK_SETTINGS__INVALID_SETTINGS:
                logger->log("Invalid network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__VALID_SETTINGS:
                logger->log("Valid network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__CONVERTED_SETTINGS:
                logger->log("Converted network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__DEFAULT_SETTINGS:
                logger->log("Default network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__USING_SETTINGS:
                logger->log("Using network settings");
                break;

            case BB_Event::MALLOC__FAILED:
                logger->log("Failed to allocate memory");
                break;

            default:
                logger->logf("Unknown event: %d", event);
                break;

        }
    }
}
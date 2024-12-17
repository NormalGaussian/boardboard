#include "EventLogger.hpp"

#include <functional>
using BB_Logger::LogLevel;

namespace BB_Event {

    EventLogger::EventLogger(std::weak_ptr<Logger> logger) {
        this->logger = logger;
    }

    EventLogger::~EventLogger() {}

    std::shared_ptr<EventLogger> EventLogger::create(std::weak_ptr<Logger> logger) {
        auto result = std::make_shared<EventLogger>(logger);
        auto thiz = result.get();
        std::function<void(Event)> onEvent = [thiz](Event event) {
            (thiz->onEvent)(event);
        };
        thiz->registration = Registration::create(onEvent, ALL);
        return result;
    }

    void EventLogger::onEvent(BB_Event::Event event) {
        auto logger = this->logger.lock();
        if(!logger) {
            return;
        }

        switch(event) {
            case BB_Event::NETWORK_SETTINGS__LOADED:
                logger->log("Event: Network settings loaded");
                break;
            case BB_Event::NETWORK_SETTINGS__FAILED_LOAD:
                logger->log("Event: Failed to load network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__SAVED:
                logger->log("Event: Network settings saved");
                break;
            case BB_Event::NETWORK_SETTINGS__FAILED_SAVE:
                logger->log("Event: Failed to save network settings");
                break;

            case BB_Event::NETWORK_SETTINGS__INVALID_SETTINGS:
                logger->log("Event: Invalid network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__VALID_SETTINGS:
                logger->log("Event: Valid network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__CONVERTED_SETTINGS:
                logger->log("Event: Converted network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__DEFAULT_SETTINGS:
                logger->log("Event: Default network settings");
                break;
            case BB_Event::NETWORK_SETTINGS__USING_SETTINGS:
                logger->log("Event: Using network settings");
                break;

            case BB_Event::NETWORK__SET_CONNECTION:
                logger->log("Event: Set connection");
                break;
            case BB_Event::NETWORK__FAILED_SET_CONNECTION:
                logger->log("Event: Failed to set connection");
                break;
            case BB_Event::NETWORK__CONNECTED:
                logger->log("Event: Connected");
                break;
            case BB_Event::NETWORK__FAILED_CONNECT_TIMEOUT:
                logger->log("Event: Failed to connect: Timeout");
                break;
            case BB_Event::NETWORK__FAILED_CONNECT_INVALID:
                logger->log("Event: Failed to connect: Invalid");
                break;
            case BB_Event::NETWORK__DISCONNECTED:
                logger->log("Event: Disconnected");
                break;
            case BB_Event::NETWORK__SET_HOST:
                logger->log("Event: Set host");
                break;
            case BB_Event::NETWORK__FAILED_SET_HOST:
                logger->log("Event: Failed to set host");
                break;
            case BB_Event::NETWORK__HOSTED:
                logger->log("Event: Hosted");
                break;
            case BB_Event::NETWORK__FAILED_HOST_TIMEOUT:
                logger->log("Event: Failed to host: Timeout");
                break;
            case BB_Event::NETWORK__FAILED_HOST_INVALID:
                logger->log("Event: Failed to host: Invalid");
                break;
            case BB_Event::NETWORK__STOPPED_HOSTING:
                logger->log("Event: Stopped hosting");
                break;
            case BB_Event::NETWORK__GET_NETWORK:
                logger->log("Event: Get network");
                break;
            case BB_Event::NETWORK__INITIALISED:
                logger->log("Event: Initialised network");
                break;
            case BB_Event::NETWORK__DESTROYED:
                logger->log("Event: Destroyed network");
                break;
            
            case BB_Event::DISPLAY__INITIALISED:
                logger->log("Event: Initialised display");
                break;
            case BB_Event::DISPLAY__DESTROYED:
                logger->log("Event: Destroyed display");
                break;
            case BB_Event::DISPLAY__RESET:
                logger->log("Event: Reset display");
                break;
            case BB_Event::DISPLAY__GET_DISPLAY:
                logger->log("Event: Get display");
                break;
            
            case BB_Event::SERIAL__INITIALISED:
                logger->log("Event: Initialised serial");
                break;
            case BB_Event::SERIAL__DESTROYED:
                logger->log("Event: Destroyed serial");
                break;
            case BB_Event::SERIAL__GET_SERIAL_CONNECTION:
                logger->log("Event: Get serial connection");
                break;

            case BB_Event::MALLOC__FAILED:
                logger->log("Event: Failed to allocate memory");
                break;

            default:
                logger->logf("Event: Unknown event: %d", event);
                break;

        }
    }
}
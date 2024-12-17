#pragma once
#include <functional>
#include <memory>

namespace BB_Event
{
    enum Event
    {
        ALL, // Should not be emitted, only used for registering all events

        NETWORK_SETTINGS__LOADED,
        NETWORK_SETTINGS__FAILED_LOAD,
        NETWORK_SETTINGS__SAVED,
        NETWORK_SETTINGS__FAILED_SAVE,
        
        NETWORK_SETTINGS__INVALID_SETTINGS,
        NETWORK_SETTINGS__VALID_SETTINGS,
        NETWORK_SETTINGS__CONVERTED_SETTINGS,
        NETWORK_SETTINGS__DEFAULT_SETTINGS,
        NETWORK_SETTINGS__USING_SETTINGS,

        NETWORK__SET_CONNECTION,
        NETWORK__FAILED_SET_CONNECTION,
        NETWORK__CONNECTED,
        NETWORK__FAILED_CONNECT_TIMEOUT,
        NETWORK__FAILED_CONNECT_INVALID,
        NETWORK__DISCONNECTED,
        NETWORK__SET_HOST,
        NETWORK__FAILED_SET_HOST,
        NETWORK__HOSTED,
        NETWORK__FAILED_HOST_TIMEOUT,
        NETWORK__FAILED_HOST_INVALID,
        NETWORK__STOPPED_HOSTING,
        NETWORK__GET_NETWORK,
        NETWORK__INITIALISED,
        NETWORK__DESTROYED,

        DISPLAY__INITIALISED,
        DISPLAY__DESTROYED,
        DISPLAY__RESET,
        DISPLAY__GET_DISPLAY,

        SERIAL__INITIALISED,
        SERIAL__DESTROYED,
        SERIAL__GET_SERIAL_CONNECTION,

        MALLOC__FAILED,
    };
    
    using EventHandler = std::function<void(Event)>;

    void event(Event event);

    class Registration {
    public:
        EventHandler callback;
        Event event;
        Registration(EventHandler callback, Event event);
        ~Registration();
        static std::shared_ptr<Registration> create(EventHandler callback, Event event);
    };
};

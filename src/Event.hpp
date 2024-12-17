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

        MALLOC__FAILED,
    };
    
    using EventHandler = void(*)(Event);

    void event(Event event);

    class Registration {
    private:
        EventHandler callback;
    public:
        Registration(EventHandler callback, Event event);
        ~Registration();
        static std::shared_ptr<Registration> create(EventHandler callback, Event event);
    };
};

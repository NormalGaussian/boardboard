#include "Event.hpp"
#include <map>
#include <list>
#include <set>

namespace BB_Event
{
    std::map<Event, std::set<EventHandler>> eventMap = {};
    std::map<EventHandler, std::set<Event>> handlerMap = {};

    void event(Event event) {
        // Handlers for the specific event
        auto eventHandlers = eventMap.find(event);
        if(eventHandlers != eventMap.end()) {
            for(EventHandler eventHandler : eventHandlers->second) {
                (*eventHandler)(event);
            }
        }

        // Handlers for all events
        auto allHandlers = eventMap.find(ALL);
        if(allHandlers != eventMap.end()) {
            for(EventHandler eventHandler : allHandlers->second) {
                (*eventHandler)(event);
            }
        }
    };
    
    void registerEventHandler(EventHandler callback, Event event) {
        // Update eventMap
        if(eventMap.find(event) == eventMap.end()) {
            eventMap[event] = {};
        }
        eventMap[event].insert(callback);
        
        // Update handlerMap
        if(handlerMap.find(callback) == handlerMap.end()) {
            handlerMap[callback] = {};
        }
        handlerMap[callback].insert(event);
    }

    void unregisterEventHandler(EventHandler callback) {
        if(handlerMap.find(callback) == handlerMap.end()) {
            return;
        }

        // Remove callback from eventMap
        for(Event event : handlerMap[callback]) {
            eventMap[event].erase(callback);
        }
        
        // Remove callback from handlerMap
        handlerMap.erase(callback);
    }

    Registration::Registration(EventHandler callback, Event event) {
        registerEventHandler(callback, event);
        this->callback = callback;
    };
    Registration::~Registration() {
        unregisterEventHandler(this->callback);
    };
    std::shared_ptr<Registration> Registration::create(EventHandler callback, Event event) {
        return std::make_shared<Registration>(callback, event);
    }
}
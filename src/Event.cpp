#include "Event.hpp"
#include <map>
#include <list>
#include <set>
#include <memory>

namespace BB_Event
{
    std::list<std::weak_ptr<Registration>> registrations = {};
    
    Registration::Registration(EventHandler callback, Event event) {
        this->callback = callback;
        this->event = event;
    };
    Registration::~Registration() {
        for(auto it = registrations.begin(); it != registrations.end(); it++) {
            auto shared = it->lock();
            Registration *thiz = shared.get();
            if(thiz == this) {
                registrations.erase(it);
                break;
            }
        }
    };
    std::shared_ptr<Registration> Registration::create(EventHandler callback, Event event) {
        auto shared = std::make_shared<Registration>(callback, event);
        registrations.push_back(shared);
        return shared;
    }

    void event(Event event) {
        for(auto registration : registrations) {
            auto shared = registration.lock();
            if(shared && (shared->event == event || shared->event == ALL)) {
                shared->callback(event);
            }
        }
    }
}
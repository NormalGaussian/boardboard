#include "SerialConnection.hpp"
#include <Arduino.h>
#include "Event.hpp"

using BB_Event::Event;
using BB_Event::event;

namespace BB_Serial {
        SerialConnection::SerialConnection() {
            Serial.begin(9600);
            event(Event::SERIAL__INITIALISED);
        }
        SerialConnection::~SerialConnection() {
            Serial.end();
            event(Event::SERIAL__DESTROYED);
        }
        HardwareSerial *SerialConnection::getSerial() {
            return &Serial;
        }
        std::weak_ptr<SerialConnection> serialConnection;
        std::shared_ptr<SerialConnection> getSerialConnection() {
            event(Event::SERIAL__GET_SERIAL_CONNECTION);
            auto locked = serialConnection.lock();
            if(locked) {
                return locked;
            }

            auto connection = std::make_shared<SerialConnection>();
            serialConnection = connection;
            return connection;
        }
};

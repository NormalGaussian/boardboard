#include "SerialConnection.hpp"
#include <Arduino.h>

namespace BB_Serial {
        SerialConnection::SerialConnection() {
            Serial.begin(9600);
        }
        SerialConnection::~SerialConnection() {
            Serial.end();
        }
        HardwareSerial *SerialConnection::getSerial() {
            return &Serial;
        }
        std::weak_ptr<SerialConnection> serialConnection;
        std::shared_ptr<SerialConnection> getSerialConnection() {
            auto locked = serialConnection.lock();
            if(locked) {
                return locked;
            }

            auto connection = std::make_shared<SerialConnection>();
            serialConnection = connection;
            return connection;
        }
};

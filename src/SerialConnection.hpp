#pragma once

#include <memory>
#include <Arduino.h>

namespace BB_Serial {
    class SerialConnection {
    public:
        SerialConnection();
        ~SerialConnection();
        HardwareSerial *getSerial();
    };
    std::shared_ptr<SerialConnection> getSerialConnection();
};

#pragma once

#include <WiFi.h>
#include <HTTPClient.h>
#include "Logger.hpp"

#include "Event.hpp"
using BB_Event::event;
using BB_Event::Event;
using BB_Logger::defaultLogger;

namespace BB_Messages {
    char messages[512];

    bool update() {
        auto logger = defaultLogger;
        
        HTTPClient http;
        String url = "https://oatmealstuffing.com/projects/boardboard/messages?id=" + String(ESP.getEfuseMac());
        logger->infof("Connecting to %s", url.c_str());
        http.begin(url);
        auto httpCode = http.GET();
        auto size = http.getSize();
        logger->infof("Code: %d; Size: %d", httpCode, size);

        http.getStream().readBytes(messages, sizeof(messages));
        //event(Event::MESSAGES__UPDATED);
        return httpCode == 200;
    }

};

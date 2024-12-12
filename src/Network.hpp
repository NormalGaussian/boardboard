#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include "Display.hpp"
#include <EEPROM.h>
#include "NetworkSettings.hpp"

char sentinel[8] = "BB:.:NS";

namespace BB_Network
{

    enum Mode {
        CLIENT,
        HOST,
        SCANNER,
        OFF
    };

    class Network
    {
    private:
        void WiFiEventHandler(WiFiEvent_t event);

        Mode mode = Mode::OFF;

        /**
         * Settings to use when hosting a network. 
         */
        BB_NetworkSettings::NetworkSettings_v1 hosted_network_settings;

        /**
         * Settings to use when connecting to a network.
         */
        BB_NetworkSettings::NetworkSettings_v1 connected_network_settings;

        int constructor(int address_offset);

        bool saveSettings();
        bool loadSettings();

    public:
        Network();
        Network(int address_offset);
        ~Network();

        bool connect();
        bool connect(const char *ssid, const char *password);
        void disconnect();

        void host(const char *ssid, const char *password);
        void stopHosting();

        void serve(const char *path, const char *content, const char *contentType);
        void handle(const char *path, void (*handler)(WiFiClient client));

        char *IP();
    };

    /**
     * The device only has one network setup, so it should be a singleton.
     */
    Network network = Network();

};

#endif

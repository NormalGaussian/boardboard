#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include "Display.hpp"
#include <EEPROM.h>
#include "NetworkSettings.hpp"

using BB_NetworkSettings::NetworkSettings_v1;

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
        NetworkSettings_v1 hosted_network_settings;

        /**
         * Settings to use when connecting to a network.
         */
        NetworkSettings_v1 connected_network_settings;

        bool constructor(int address_offset);

        bool saveSettings();
        bool loadSettings();

    public:
        Network();
        Network(int address_offset);
        ~Network();

        Mode getMode() {
            return mode;
        }

        bool updateConnection(const char *ssid, const char *password);
        bool connect();
        bool connect(const char *ssid, const char *password);
        void disconnect();

        bool updateHost(const char *ssid, const char *password);
        bool host();
        bool host(const char *ssid, const char *password);
        void stopHosting();

        String IP();
    };

    /**
     * The device only has one network setup, so it should be a singleton.
     */
    extern Network network;

};

#endif

#include "Network.hpp"
#include "NetworkSettings.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "Event.hpp"

#define CONNECTED_NETWORK_SETTINGS_ADDRESS 200
#define HOSTED_NETWORK_SETTINGS_ADDRESS 300

using BB_NetworkSettings::NetworkSettings;
using BB_NetworkSettings::NetworkSettings_v1;
using namespace BB_NetworkSettings;
using namespace BB_Utils;
using BB_Logger::defaultLogger;
using BB_Logger::Logger;
using BB_Logger::LogLevel;
using BB_Event::event;
using BB_Event::Event;

namespace BB_Network
{
    const char *ssid = "REPLACE_WITH_YOUR_SSID";
    const char *password = "REPLACE_WITH_YOUR_PASSWORD";

    void Network::WiFiEventHandler(WiFiEvent_t event)
    {
        auto logger = defaultLogger;

        logger->logf("[WiFi-event] event: %d\n", event);

        switch (event)
        {
        case ARDUINO_EVENT_WIFI_READY:
            logger->log("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            logger->log("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            logger->log("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            logger->log("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            logger->log("Connected to access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            logger->log("Disconnected from WiFi access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            logger->log("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            logger->logf("Obtained IP address: %s", WiFi.localIP().toString().c_str());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            logger->log("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            logger->log("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            logger->log("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            logger->log("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            logger->log("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            logger->log("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            logger->log("WiFi access point stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            logger->log("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            logger->log("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            logger->log("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            logger->log("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            logger->log("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            logger->log("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            logger->log("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            logger->log("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            logger->log("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            logger->log("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            logger->log("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            logger->log("Obtained IP address");
            break;
        default:
            break;
        }
    }

    bool Network::loadSettings()
    {
        bool result = true;
        NetworkSettings union_tmp;

        // Load connected network settings
        if (BB_NetworkSettings::load(CONNECTED_NETWORK_SETTINGS_ADDRESS, &union_tmp) && BB_NetworkSettings::convertTo_v1(&union_tmp))
        {
            memcpy(&connected_network_settings, &union_tmp.v1, sizeof(NetworkSettings_v1));
        }
        else
        {
            result = false;
        }

        // Load hosted network settings
        if (BB_NetworkSettings::load(HOSTED_NETWORK_SETTINGS_ADDRESS, &union_tmp) && BB_NetworkSettings::convertTo_v1(&union_tmp))
        {
            memcpy(&hosted_network_settings, &union_tmp.v1, sizeof(NetworkSettings_v1));
        }
        else
        {
            result = false;
        }

        return result;
    }

    bool Network::saveSettings()
    {
        bool savedConnected = BB_NetworkSettings::save(CONNECTED_NETWORK_SETTINGS_ADDRESS, &connected_network_settings);
        bool savedHosted = BB_NetworkSettings::save(HOSTED_NETWORK_SETTINGS_ADDRESS, &hosted_network_settings);

        return savedConnected && savedHosted;
    }

    bool Network::constructor(int address_offset)
    {
        // delete old config
        WiFi.disconnect(true);
        WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                     { this->WiFiEventHandler(event); });

        return loadSettings();
    }

    Network::Network::Network(int address_offset)
    {
        event(Event::NETWORK__INITIALISED);
        constructor(address_offset);
    }

    Network::~Network()
    {
        /**
         * Don't do any saving here - we can't be sure this will be called
         * Saving should be done as and when something saveable happens!
         */

        // Shut down wifi (erasing credentials)
        WiFi.disconnect(true, true);
        event(Event::NETWORK__DESTROYED);
    }

    bool Network::connect(int timeout_s)
    {

        NetworkSettings_v1 *settings = &connected_network_settings;

        if (settings->ssid[0] == '\0' || settings->password[0] == '\0')
        {
            // No settings to connect with
            event(Event::NETWORK__FAILED_CONNECT_INVALID);
            return false;
        }

        mode = Mode::CLIENT;

        WiFi.begin(settings->ssid, settings->password);

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < timeout_s)
        {
            delay(1000);
            attempts++;
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            disconnect();
            event(Event::NETWORK__FAILED_CONNECT_TIMEOUT);
            return false;
        }
        event(Event::NETWORK__CONNECTED);
        return true;
    }

    bool Network::connected()
    {
        return WiFi.status() == WL_CONNECTED;
    }

    bool Network::updateConnection(const char *ssid, const char *password)
    {
        NetworkSettings_v1 newSettings;
        BB_NetworkSettings::reset(&newSettings);

        bool result = true;
        result = result && BB_Utils::copyNonEmptyString(newSettings.ssid, ssid, sizeof(newSettings.ssid));
        result = result && BB_Utils::copyNonEmptyString(newSettings.password, password, sizeof(newSettings.password));

        if (result)
        {
            memcpy(&connected_network_settings, &newSettings, sizeof(NetworkSettings_v1));
            result = result && BB_NetworkSettings::save(CONNECTED_NETWORK_SETTINGS_ADDRESS, &connected_network_settings);
        }

        if(result) {
            event(Event::NETWORK__SET_CONNECTION);
        } else {
            event(Event::NETWORK__FAILED_SET_CONNECTION);
        }

        return result;
    }

    bool Network::connect(const char *ssid, const char *password, int timeout_s)
    {
        return updateConnection(ssid, password) && connect(timeout_s);
    }

    void Network::disconnect()
    {
        WiFi.disconnect(true, true);
        event(Event::NETWORK__DISCONNECTED);
        mode = Mode::OFF;
    }

    bool Network::host(int timeout_s)
    {
        NetworkSettings_v1 *settings = &hosted_network_settings;
        if (settings->ssid[0] == '\0' || settings->password[0] == '\0')
        {
            event(Event::NETWORK__FAILED_HOST_INVALID);
            return false;
        }

        mode = Mode::HOST;

        WiFi.softAP(settings->ssid, settings->password);
        int attempts = 0;
        while (WiFi.softAPIP() == INADDR_NONE && attempts < timeout_s)
        {
            delay(1000);
            attempts++;
        }
        if (WiFi.softAPIP() == INADDR_NONE)
        {
            stopHosting();
            event(Event::NETWORK__FAILED_HOST_TIMEOUT);
            return false;
        }

        event(Event::NETWORK__HOSTED);
        return true;
    }

    bool Network::updateHost(const char *ssid, const char *password)
    {
        NetworkSettings_v1 newSettings;
        BB_NetworkSettings::reset(&newSettings);

        bool result = true;
        result = result && BB_Utils::copyNonEmptyString(newSettings.ssid, ssid, sizeof(newSettings.ssid));
        result = result && BB_Utils::copyNonEmptyString(newSettings.password, password, sizeof(newSettings.password));

        if (result)
        {
            memcpy(&hosted_network_settings, &newSettings, sizeof(NetworkSettings_v1));
            result = result && BB_NetworkSettings::save(HOSTED_NETWORK_SETTINGS_ADDRESS, &hosted_network_settings);
        }
        
        if(result) {
            event(Event::NETWORK__SET_HOST);
        } else {
            event(Event::NETWORK__FAILED_SET_HOST);
        }

        return result;
    }

    bool Network::host(const char *ssid, const char *password, int timeout_s)
    {
        return updateHost(ssid, password) && host(timeout_s);
    }

    void Network::stopHosting()
    {
        WiFi.softAPdisconnect(true);
        event(Event::NETWORK__STOPPED_HOSTING);
        mode = Mode::OFF;
    }

    String Network::IP()
    {
        switch (mode)
        {
        case Mode::CLIENT:
            return WiFi.localIP().toString();
        case Mode::HOST:
            return WiFi.softAPIP().toString();
        default:
            return "";
        }
    }

    std::shared_ptr<Network> network = nullptr;
    std::shared_ptr<Network> getNetwork(int address_offset)
    {
        event(Event::NETWORK__GET_NETWORK);
        if (network == nullptr)
        {
            network = std::make_shared<Network>(address_offset);
        }
        return network;
    }

    bool Network::hasConnectionDetails()
    {
        return connected_network_settings.ssid[0] != '\0';
    }

    bool Network::hasHostDetails()
    {
        return hosted_network_settings.ssid[0] != '\0';
    }
}
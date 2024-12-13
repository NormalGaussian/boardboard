#include "Network.hpp"
#include "NetworkSettings.hpp"
#include "Utils.hpp"

#define CONNECTED_NETWORK_SETTINGS_ADDRESS 0
#define HOSTED_NETWORK_SETTINGS_ADDRESS    100

using BB_NetworkSettings::NetworkSettings_v1;
using BB_NetworkSettings::NetworkSettings;
using namespace BB_NetworkSettings;
using namespace BB_Utils;
using BB_Display::display;

namespace BB_Network
{
    const char *ssid = "REPLACE_WITH_YOUR_SSID";
    const char *password = "REPLACE_WITH_YOUR_PASSWORD";

    void Network::WiFiEventHandler(WiFiEvent_t event)
    {
        display.reset();
        display.printlnf("[WiFi-event] event: %d\n", event);

        switch (event)
        {
        case ARDUINO_EVENT_WIFI_READY:
            display.printlnf("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            display.printlnf("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            display.printlnf("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            display.printlnf("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            display.printlnf("Connected to access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            display.printlnf("Disconnected from WiFi access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            display.printlnf("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            display.printlnf("Obtained IP address: %s", WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            display.printlnf("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            display.printlnf("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            display.printlnf("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            display.printlnf("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            display.printlnf("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            display.printlnf("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            display.printlnf("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            display.printlnf("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            display.printlnf("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            display.printlnf("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            display.printlnf("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            display.printlnf("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            display.printlnf("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            display.printlnf("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            display.printlnf("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            display.printlnf("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            display.printlnf("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            display.printlnf("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            display.printlnf("Obtained IP address");
            break;
        default:
            break;
        }
    }

    bool Network::loadSettings() {
        bool result = true;
        NetworkSettings union_tmp;
        
        // Load connected network settings
        if(BB_NetworkSettings::load(CONNECTED_NETWORK_SETTINGS_ADDRESS, &union_tmp) && BB_NetworkSettings::convertTo_v1(&union_tmp)) {
            memcpy(&connected_network_settings, &union_tmp.v1, sizeof(NetworkSettings_v1));
        } else {
            result = false;
        }

        // Load hosted network settings
        if(BB_NetworkSettings::load(HOSTED_NETWORK_SETTINGS_ADDRESS, &union_tmp) && BB_NetworkSettings::convertTo_v1(&union_tmp)) {
            memcpy(&hosted_network_settings, &union_tmp.v1, sizeof(NetworkSettings_v1));
        } else {
            result = false;
        }

        return result;
    }

    bool Network::saveSettings() {
        bool savedConnected = BB_NetworkSettings::save(CONNECTED_NETWORK_SETTINGS_ADDRESS, &connected_network_settings);
        bool savedHosted = BB_NetworkSettings::save(HOSTED_NETWORK_SETTINGS_ADDRESS, &hosted_network_settings);

        return savedConnected && savedHosted;
    }

    bool Network::constructor(int address_offset) {
        // delete old config
        WiFi.disconnect(true);
        WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info)
                     { network.WiFiEventHandler(event); });

        return loadSettings();
    }

    Network::Network() {
        constructor(0);
    }

    Network::Network::Network(int address_offset)
    {
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
    }

    bool Network::connect() {
        mode = Mode::CLIENT;

        NetworkSettings_v1 *settings = &connected_network_settings;

        display.reset();
        display.printlnf("Connecting to %s", settings->ssid);

        WiFi.begin(settings->ssid, settings->password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
            display.printlnf("Connecting...");

            // TODO: Add timeout & false return
        }

        display.printlnf("Connected to %s", settings->ssid);
        
        display.printlnf("IP: %s", IP());

        return true;
    }

    bool Network::updateConnection(const char *ssid, const char *password) {
        NetworkSettings_v1 newSettings;
        BB_NetworkSettings::reset(&newSettings);
        
        bool result = true;
        result = result && BB_Utils::copyNonEmptyString(newSettings.ssid, ssid, sizeof(newSettings.ssid));
        result = result && BB_Utils::copyNonEmptyString(newSettings.password, password, sizeof(newSettings.password));

        if(result) {
            memcpy(&connected_network_settings, &newSettings, sizeof(NetworkSettings_v1));
            result = result && saveSettings();
        }

        return result;
    }

    bool Network::connect(const char *ssid, const char *password)
    {
        return updateConnection(ssid, password) && connect();
    }

    void Network::disconnect()
    {
        WiFi.disconnect(true, true);
        mode = Mode::OFF;
    }

    bool Network::host()
    {
        NetworkSettings_v1 *settings = &hosted_network_settings;

        mode = Mode::HOST;
        display.reset();
        display.printlnf("Hosting %s", settings->ssid);

        WiFi.softAP(settings->ssid, settings->password);
        while(WiFi.softAPIP() == INADDR_NONE) {
            delay(1000);
            display.printlnf("Hosting...");
        }

        display.printlnf("Hosted %s", settings->ssid);

        display.printlnf("IP: %s", IP());

        return true;
    }

    bool Network::updateHost(const char *ssid, const char *password) {
        NetworkSettings_v1 newSettings;
        BB_NetworkSettings::reset(&newSettings);
        
        bool result = true;
        result = result && BB_Utils::copyNonEmptyString(newSettings.ssid, ssid, sizeof(newSettings.ssid));
        result = result && BB_Utils::copyNonEmptyString(newSettings.password, password, sizeof(newSettings.password));

        if(result) {
            memcpy(&hosted_network_settings, &newSettings, sizeof(NetworkSettings_v1));
            result = result && saveSettings();
        }

        return result;
    }

    bool Network::host(const char *ssid, const char *password)
    {
        return updateHost(ssid, password) && host();
    }

    void Network::stopHosting()
    {
        WiFi.softAPdisconnect(true);
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

    Network network = Network();
}
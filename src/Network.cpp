#include "Network.hpp"
#include "NetworkSettings.hpp"

#define TARGET_SETTINGS_ADDRESS 0
#define OWN_SETTINGS_ADDRESS    100

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

    bool Network::Network::loadSettings() {
        // Read stored target SSID and password from EEPROM
        BB_NetworkSettings::NetworkSettings storedTargetSettings;
        bool loadedTargetSettings = BB_NetworkSettings::load(TARGET_SETTINGS_ADDRESS, &storedTargetSettings);
         
        
        if(loadedTargetSettings) {
            strcpy(target_ssid, targetSettings.v1.ssid);
            strcpy(target_password, targetSettings.v1.password);
        }

        // Read stored own SSID and password from EEPROM
        BB_NetworkSettings::NetworkSettings ownSettings;
        bool loadedOwnSettings = BB_NetworkSettings::load(OWN_SETTINGS_ADDRESS, &ownSettings);
        if(loadedOwnSettings) {
            strcpy(own_ssid, ownSettings.v1.ssid);
            strcpy(own_password, ownSettings.v1.password);
        }

        return true;
    }

    bool Network::Network::saveSettings() {
        // Save target SSID and password to EEPROM
        BB_NetworkSettings::NetworkSettings targetSettings;
        targetSettings.v1.version = 1;
        strcpy(targetSettings.v1.ssid, target_ssid);
        strcpy(targetSettings.v1.password, target_password);
        bool savedTargetSettings = BB_NetworkSettings::save(TARGET_SETTINGS_ADDRESS, &targetSettings);

        // Save own SSID and password to EEPROM
        BB_NetworkSettings::NetworkSettings ownSettings;
        ownSettings.v1.version = 1;
        strcpy(ownSettings.v1.ssid, own_ssid);
        strcpy(ownSettings.v1.password, own_password);
        bool savedOwnSettings = BB_NetworkSettings::save(OWN_SETTINGS_ADDRESS, &ownSettings);

        return savedTargetSettings && savedOwnSettings;
    }

    int Network::Network::constructor(int address_offset) {
        // delete old config
        WiFi.disconnect(true);
        WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info)
                     { network.WiFiEventHandler(event); });

        loadSettings();
    }

    Network::Network::Network() {
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

    bool connect() {
        display.reset();
        display.printlnf("Connecting to %s", target_ssid);

        WiFi.begin(target_ssid, target_password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
            display.printlnf("Connecting...");
        }

        // Save 

        display.printlnf("Connected to %s", target_ssid);
    }

    bool connect(const char *ssid, const char *password)
    {
        display.reset();
        display.printlnf("Connecting to %s", ssid);

        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
            display.printlnf("Connecting...");
        }
        

        display.printlnf("Connected to %s", ssid);
    }
}
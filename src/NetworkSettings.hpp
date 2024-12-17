#ifndef NETWORK_SETTINGS_H
#define NETWORK_SETTINGS_H

#include <stdlib.h>

namespace BB_NetworkSettings
{
    extern char sentinel[8];

    // All versions are expected to be compatible with this one
    struct NetworkSettings_v0
    {
        uint8_t version;
    } __attribute__((packed)) __attribute__((aligned(4)));

    struct NetworkSettings_v1
    {
        uint8_t version;
        char ssid[32];
        char password[64];
    } __attribute__((packed)) __attribute__((aligned(4)));

    // I've included this version to show how the versioning would be handled.
    struct NetworkSettings_v2
    {
        uint8_t version;
        char ssid[32];
        char password[64];
        char hostname[32];
    } __attribute__((packed)) __attribute__((aligned(4)));

    union NetworkSettings
    {
        NetworkSettings_v0 v0;
        NetworkSettings_v1 v1;
        NetworkSettings_v2 v2;
    };

    void reset(NetworkSettings_v0 *networkSettings);
    void reset(NetworkSettings_v1 *networkSettings);
    void reset(NetworkSettings_v2 *networkSettings);
    void reset(NetworkSettings *networkSettings);

    bool load(int address, NetworkSettings_v0 *networkSettings);
    bool load(int address, NetworkSettings_v1 *networkSettings);
    bool load(int address, NetworkSettings_v2 *networkSettings);
    bool load(int address, NetworkSettings *networkSettings);

    bool save(int address, NetworkSettings_v0 *networkSettings);
    bool save(int address, NetworkSettings_v1 *networkSettings);
    bool save(int address, NetworkSettings_v2 *networkSettings);
    bool save(int address, NetworkSettings *networkSettings);

    bool convertTo_v0(NetworkSettings *networkSettings);
    bool convertTo_v1(NetworkSettings *networkSettings);
    bool convertTo_v2(NetworkSettings *networkSettings);
}

#endif

#include "NetworkSettings.hpp"
#include "Event.hpp"
#include "Utils.hpp"

using BB_Event::Event;
using BB_Event::event;
using BB_Utils::RAII_Malloc;
using BB_Utils::xorChecksum;
using BB_Utils::isNullTerminated;

namespace BB_NetworkSettings
{
    /**
     * Check if the buffer is a sentinel
     */
    bool isSentinel(char *buffer)
    {
        return strncmp(buffer, sentinel, sizeof(sentinel)) == 0;
    }
    
    bool validateSettings(NetworkSettings_v0 *settings)
    {
        if (settings->version != 0)
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }
        event(Event::NETWORK_SETTINGS__VALID_SETTINGS);
        return true;
    }

    /**
     * Validate NetworkSettings_v1
     */
    bool validateSettings(NetworkSettings_v1 *settings)
    {
        if (settings->version != 1)
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }

        if (!isNullTerminated(settings->ssid, sizeof(NetworkSettings_v1::ssid)))
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }

        if (!isNullTerminated(settings->password, sizeof(NetworkSettings_v1::password)))
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }

        event(Event::NETWORK_SETTINGS__VALID_SETTINGS);
        return true;
    }

    /**
     * Validate NetworkSettings_v2
     */
    bool validateSettings(NetworkSettings_v2 *settings)
    {
        if (settings->version != 2)
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }

        if (!isNullTerminated(settings->ssid, sizeof(NetworkSettings_v2::ssid)))
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }

        if (!isNullTerminated(settings->password, sizeof(NetworkSettings_v2::password)))
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }

        if (!isNullTerminated(settings->hostname, sizeof(NetworkSettings_v2::hostname)))
        {
            event(Event::NETWORK_SETTINGS__INVALID_SETTINGS);
            return false;
        }

        event(Event::NETWORK_SETTINGS__VALID_SETTINGS);
        return true;
    }

    /**
     * Validate NetworkSettings
     */
    bool validateSettings(NetworkSettings *settings)
    {
        // Read the version and start the validation
        //  we don't need to check the sentinel here
        //  as it is checked in the version specific
        //  validation functions.
        switch (settings->v0.version)
        {
        case 0:
            return validateSettings(&settings->v0);
        case 1:
            return validateSettings(&settings->v1);
        case 2:
            return validateSettings(&settings->v2);
        default:
            return false;
        }
    }

    /**
     * Calculate the size required to store the data with sentinels and checksum
     */
    size_t required_size(size_t size) {
        size_t required_space = 0;
        required_space += size;
        required_space += 2 * sizeof(sentinel);
        required_space += 1; // checksum
        return required_space;
    }

    /**
     * Save a structure to EEPROM with sentinels and checksum
     */
    template<typename T>
    bool saveToEEPROM(int address, const T& structure) {
        size_t size = required_size(sizeof(T));
        RAII_Malloc buffer(size);
        if(buffer.failed_to_allocate()) {
            event(Event::NETWORK_SETTINGS__FAILED_SAVE);
            return false;
        }

        char checksum = xorChecksum(structure, sizeof(T));

        // Copy the data into the buffer, adding sentinels and checksum
        memcpy(buffer, sentinel, sizeof(sentinel));
        memcpy(buffer + sizeof(sentinel), &structure, sizeof(T));
        memcpy(buffer + sizeof(sentinel) + sizeof(T), &checksum, 1);
        memcpy(buffer + sizeof(sentinel) + sizeof(T) + 1, sentinel, sizeof(sentinel));

        // Write
        EEPROM.begin(size);
        EEPROM.writeBytes(address, buffer, size);
        EEPROM.commit();
        EEPROM.end();
        
        // Read back to ensure it was written correctly
        RAII_Malloc readback(size);
        if(readback.failed_to_allocate()) {
            event(Event::NETWORK_SETTINGS__FAILED_SAVE);
            return false;
        }
        EEPROM.begin(size);
        EEPROM.readBytes(address, readback, size);
        EEPROM.end();

        // Compare
        bool result = memcmp(structure, readback, size) == 0;

        if(!result) {
            event(Event::NETWORK_SETTINGS__FAILED_SAVE);
        } else {
            event(Event::NETWORK_SETTINGS__SAVED);
        }

        return result;
    }

    bool save(int address, NetworkSettings_v0 *settings)
    {
        if(!validateSettings(settings)) {
            event(Event::NETWORK_SETTINGS__FAILED_SAVE);
            return false;
        }
        return saveToEEPROM(address, *settings);
    }
    bool save(int address, NetworkSettings_v1 *settings)
    {
        if(!validateSettings(settings)) {
            event(Event::NETWORK_SETTINGS__FAILED_SAVE);
            return false;
        }
        return saveToEEPROM(address, *settings);
    }
    bool save(int address, NetworkSettings_v2 *settings)
    {
        if(!validateSettings(settings)) {
            event(Event::NETWORK_SETTINGS__FAILED_SAVE);
            return false;
        }
        return saveToEEPROM(address, *settings);
    }
    bool save(int address, NetworkSettings *settings)
    {
        // This instance of the templated function ensures
        //  that the union is always "cast down" to the
        //  correct struct before being saved.

        if(!validateSettings(settings)) {
            event(Event::NETWORK_SETTINGS__FAILED_SAVE);
            return false;
        }

        uint8_t version = settings->v0.version;
        switch (version) {
            case 0: return save(address, &settings->v0);
            case 1: return save(address, &settings->v1);
            case 2: return save(address, &settings->v2);

            default:
                // Unknown version - cannot save
                return false;
        }
    }

    bool loadAndValidateStruct_fromEEPROM(int address, void *settings, size_t size, bool (*valid)(void *)) {
        RAII_Malloc buffer(required_size(size));
        if(buffer.failed_to_allocate()) {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Read the stored data from EEPROM
        EEPROM.begin(size);
        EEPROM.readBytes(address, buffer, size);
        EEPROM.end();

        // Validate the sentinels
        if (!isSentinel(buffer))
        {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }
        if(!isSentinel(buffer + sizeof(sentinel) + size + 1))
        {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Validate the checksum
        char checksum = xorChecksum(buffer + sizeof(sentinel), size);
        if (checksum != buffer[sizeof(sentinel) + size])
        {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Use the validation function
        bool result = valid(settings);
        if(!result) {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Copy the data from the buffer into the structure
        memcpy(settings, buffer + sizeof(sentinel), size);
        event(Event::NETWORK_SETTINGS__LOADED);
        return result;
    }

    bool load(int address, NetworkSettings_v0 *settings) {
        return loadAndValidateStruct_fromEEPROM(address, settings, sizeof(NetworkSettings_v0), [](void *settings) {
            return validateSettings((NetworkSettings_v0 *)settings);
        });
    }

    bool load(int address, NetworkSettings_v1 *settings) {
        return loadAndValidateStruct_fromEEPROM(address, settings, sizeof(NetworkSettings_v1), [](void *settings) {
            return validateSettings((NetworkSettings_v1 *)settings);
        });
    }

    bool load(int address, NetworkSettings_v2 *settings) {
        return loadAndValidateStruct_fromEEPROM(address, settings, sizeof(NetworkSettings_v2), [](void *settings) {
            return validateSettings((NetworkSettings_v2 *)settings);
        });
    }

    bool load(int address, NetworkSettings *structure) {
        // This ensures that the underlying struct is always
        //  *cast up* to the union. This is necessary as the
        //  saved size may differ from that of the union.

        size_t size = required_size(sizeof(NetworkSettings));
        RAII_Malloc buffer(size);
        if(buffer.failed_to_allocate()) {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Read the stored data from EEPROM
        EEPROM.begin(size);
        EEPROM.readBytes(address, buffer, size);
        EEPROM.end();

        // Validate the start sentinel
        if (!isSentinel(buffer))
        {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }
        
        uint8_t version = buffer[sizeof(sentinel)];
        if(version != 0 && version != 1 && version != 2) {
            // We don't know how to load this version
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }
        size_t settings_size = 0;
        switch (version) {
            case 0: settings_size = sizeof(NetworkSettings_v0); break;
            case 1: settings_size = sizeof(NetworkSettings_v1); break;
            case 2: settings_size = sizeof(NetworkSettings_v2); break;
            default: return false;
        }

        // Verify the checksum
        char checksum = *(buffer + sizeof(sentinel) + settings_size);
        char calculated_checksum = xorChecksum(buffer + sizeof(sentinel), settings_size);
        if(checksum != calculated_checksum) {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Validate the end sentinel
        if(!isSentinel(buffer + sizeof(sentinel) + settings_size + 1))
        {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Validate the contents
        bool result = validateSettings((NetworkSettings *) (void *) buffer + sizeof(sentinel));
        if(!result) {
            event(Event::NETWORK_SETTINGS__FAILED_LOAD);
            return false;
        }

        // Copy the data from the buffer into the structure
        memset(structure, 0, sizeof(NetworkSettings));
        memcpy(structure, buffer + sizeof(sentinel), settings_size);

        event(Event::NETWORK_SETTINGS__LOADED);
        return result;
    }



    void reset(NetworkSettings_v0 *settings)
    {
        memset(settings, 0, sizeof(NetworkSettings_v0));
        settings->version = 0;
    }
    void reset(NetworkSettings_v1 *settings)
    {
        memset(settings, 0, sizeof(NetworkSettings_v1));
        settings->version = 1;
        settings->ssid[0] = '\0';
        settings->password[0] = '\0';
    }
    void reset(NetworkSettings_v2 *settings)
    {
        memset(settings, 0, sizeof(NetworkSettings_v2));
        settings->version = 2;
        settings->ssid[0] = '\0';
        settings->password[0] = '\0';
        settings->hostname[0] = '\0';
    }
    void reset(NetworkSettings *settings)
    {
        memset(settings, 0, sizeof(NetworkSettings));
        switch(settings->v0.version) {
            case 0: reset(&settings->v0); break;
            case 1: reset(&settings->v1); break;
            case 2: reset(&settings->v2); break;
            default: break;
        }
    }

    bool convertTo_v0(NetworkSettings *settings) {
        memset(settings, 0, sizeof(NetworkSettings));
        reset(&settings->v0);
        return true;
    }
    bool convertTo_v1(NetworkSettings *settings)
    {
        switch (settings->v0.version)
        {
        case 0:
            memset(settings, 0, sizeof(NetworkSettings));
            reset(&settings->v1);
            return true;

        case 1:
            // Already in the correct version
            return true;

        case 2:
            // Convert the data
            // This is done by created a new struct, resetting it,
            //  and copying the v2 data into it. The original
            //  struct is then overwritten with the new data.
            NetworkSettings converted;
            memset(&converted, 0, sizeof(NetworkSettings));
            reset(&converted.v1);
            strcpy(converted.v1.ssid, settings->v2.ssid);
            strcpy(converted.v1.password, settings->v2.password);
            memcpy(settings, &converted, sizeof(NetworkSettings));
            return true;

        default:
            // No idea what to do with this version
            return false;
        }
    }
    bool convertTo_v2(NetworkSettings *settings)
    {
        switch (settings->v0.version)
        {
        case 0:
            memset(settings, 0, sizeof(NetworkSettings));
            reset(&settings->v2);
            return true;

        case 1:
            // Convert the data
            // This is done by created a new struct, resetting it,
            //  and copying the v1 data into it. The original
            //  struct is then overwritten with the new data.
            NetworkSettings converted;
            memset(&converted, 0, sizeof(NetworkSettings));
            reset(&converted.v2);
            strcpy(converted.v2.ssid, settings->v1.ssid);
            strcpy(converted.v2.password, settings->v1.password);
            strcpy(converted.v2.hostname, "");
            memcpy(settings, &converted, sizeof(NetworkSettings));
            return true;

        case 2:
            // Already in the correct version
            return true;

        default:
            // No idea what to do with this version
            return false;
        }
    }
}

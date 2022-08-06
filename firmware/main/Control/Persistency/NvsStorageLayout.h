#ifndef CONFIGURATION_LAYOUT_H
#define CONFIGURATION_LAYOUT_H

#include <HumiDevice.Platform/Platform.h>

// Note: At the moment only the following tyes are supported by IDF
//  - uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t
//  - zero-terminated string
//  - binary blocks
//
// float or double not supported!

/**
 * Device parameters characterize the behavior of the device and are programmed in production.
 */
struct DeviceParameters {
    uint32_t deviceSerialNumber;
};

/**
 * A custom name for wifi configuration which is already defined in IDF framework
 */
struct WifiParameters {
    char8_t ssid[32];
    char8_t password[64];

    /**
     * Checks wifi station mode (STA) configuration. It's invalid when any mendetory 
     * field is missing (eg factory reset, or NVS error)
     * 
     * @return bool true if STA configuration is valid; otherwise false
     */
    bool isWifiCfgValid() const {
        // After NVS is erased, the SSID is empty. so we can be sure there
        // is no valid configuration.
        return strlen((char*)ssid) != 0;
    }
};

#endif // CONFIGURATION_LAYOUT_H
#ifndef CONFIGURATION_LAYOUT_H
#define CONFIGURATION_LAYOUT_H

#include "HumiPlatform.h"
#include "esp_wifi_types.h"

/**
 * Device parameters characterize the behavior of the device and are programmed in production.
 */
struct DeviceParameters {
    uint32_t deviceSerialNumber;
};

/**
 * A custom name for wifi configuration which is already defined in IDF framework
 */
union WifiParameters {
    wifi_config_t config;

    /**
     * Checks wifi station mode (STA) configuration. It's invalid when any mendetory 
     * field is missing (eg factory reset, or NVS error)
     * 
     * @return bool true if STA configuration is valid; otherwise false
     */
    bool isWifiCfgValid() const {
        // After NVS is erased, the SSID is empty. so we can be sure there
        // is no valid configuration.
        return strlen((char*)config.sta.ssid) != 0;
    }
};

#endif // CONFIGURATION_LAYOUT_H
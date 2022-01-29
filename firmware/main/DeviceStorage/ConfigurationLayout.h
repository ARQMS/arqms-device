#ifndef CONFIGURATION_LAYOUT_H
#define CONFIGURATION_LAYOUT_H

#include "Platform.h"
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
typedef wifi_config_t WifiParameters;

#endif // CONFIGURATION_LAYOUT_H
#ifndef NVS_LAYOUT_H_
#define NVS_LAYOUT_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "Events/WifiSettingsEvent.h"
#include "Events/DeviceInfoEvent.h"

/**
 * Represents a nvs layout version type
 */
typedef uint8_t NvsVersion_t;

/**
 * Represents the blob NVS structure
 */
struct NvsLayout {
public:
    // Change this version on each layout change!! Ensure you implement a new upgrade strategy in upgrade() method
    const static NvsVersion_t LAYOUT_VERSION = 1;
    // Represents a invalid version
    const static NvsVersion_t INVALID_VERSION = 0;

    /**
     * Initialize layout 
     * 
     * @return esp_err_t error code
     */
    esp_err_t initialize();

    /**
     * Restore default values
     * 
     * @return esp_err_t error code
     */
    esp_err_t reset();

    /**
     * Upgrade layout to given version
     * 
     * @param oldVersion The old version 
     * @param newVersion The new version
     * @return esp_err_t error code
     */
    esp_err_t upgrade(const NvsVersion_t oldVersion, const NvsVersion_t newVersion);

    /**
     * Fetch data from NVS into variables
     * 
     * @return esp_err_t error code
     */
    esp_err_t fetch();

    /**
     * Stores given data into nvs
     * 
     * @return esp_err_t error code
     */
    esp_err_t commit();

    // Data. Must be public for read/writeConfiguration
    NvsVersion_t version;
    char8_t ssid[WifiSettingsEvent::MAX_SSID_LENGTH];
    char8_t password[WifiSettingsEvent::MAX_PWD_LENGTH];
    char8_t sn[DeviceInfoEvent::MAX_SN_LENGTH];
    char8_t channel[DeviceInfoEvent::MAX_CHANNEL_LENGTH];
    char8_t brokerUri[DeviceInfoEvent::MAX_BROKER_URI_LENGTH];
    uint32_t interval;
};


#endif // NVS_LAYOUT_H_
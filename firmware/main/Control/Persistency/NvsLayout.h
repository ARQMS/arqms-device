#ifndef NVS_LAYOUT_H_
#define NVS_LAYOUT_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "Events/WifiSettingsEvent.h"
#include "Events/DeviceSettingsEvent.h"

/**
 * Represents a nvs layout version type
 */
typedef uint8_t NvsVersion_t;

/**
 * Represents the blob NVS structure
 */
struct NvsLayout {
public:
    // Change this version on each layout change!!
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
     * Restore default values in flash
     * 
     * @return esp_err_t error code
     */
    esp_err_t restoreDefault();

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
    char8_t sn[DeviceSettingsEvent::MAX_SN_LENGTH];
    char8_t brokerUri[DeviceSettingsEvent::MAX_BROKER_URI_LENGTH];

private:
    // Helper methods
    static void setString(const char8_t* key, const char8_t* value);
    static void getString(const char8_t* key, char8_t* value, const char8_t* defaultVal, const size_t defaultLength);

    static void setU8(const char8_t* key, const uint8_t value);
    static void getU8(const char8_t* key, uint8_t* value, const uint8_t defaultVal);
};


#endif // NVS_LAYOUT_H_
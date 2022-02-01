#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include "HumiPlatform.h"
#include "ConfigurationLayout.h"

/**
 * Represents the Non-Volatile storage for humi device. It uses
 * the underlying idf component which is based on key-value pair and uses
 * a portion of main flash memory through the esp_partition
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/storage/nvs_flash.html
 */
class DeviceStorage {
public:
    /**
     * Construct a new Device Settings object
     */
    explicit DeviceStorage();

    /**
     * Destroy the Device Settings object
     */
    ~DeviceStorage();

    /**
     * must be called before any r/w operation is executed.
     * 
     * @note Abort program if NVS can not be initialized!
     */
    void initialize();

    /**
     * @return bool when storage is successfully initialized; otherwise false
     */
    bool isInitialized();

    /**
     * reset all configuration (except DeviceParameters) to factory defaults.
     */
    void factoryDefault();

    /**
     * Get the Device Parameters object
     * 
     * @return DeviceParameters 
     */
    DeviceParameters getDeviceParameters();

    /**
     * Set the Device Parameters object
     * 
     * @param param to set
     */
    void setDeviceParameters(const DeviceParameters& param);
    
    /**
     * @brief Get the Wifi Parameters object
     * 
     * @return WifiParameters 
     */
    WifiParameters getWifiParameters();

    /**
     * @brief Set the Wifi Parameters object
     * 
     * @param param 
     */
    void setWifiParameters(const WifiParameters& param);

private:
    /**
     * Construct a new Device Settings object
     * 
     * @param copy instance
     */
    DeviceStorage(const DeviceStorage& copy);

    /**
     * private assignment operator
     * 
     * @param other  instance
     * @return DeviceSettings& the assigned instance
     */
    DeviceStorage& operator=(const DeviceStorage& other);

    /**
     * 
     */
    struct NvsLayoutV1 {
        uint16_t layoutVersion = NVS_LAYOUT_VERSION_V1;
        DeviceParameters deviceParameters;

        // version specific configuration
        // TODO
    };

    // in order to extend the layout in future, a versioned layout of the EEPROM structure planned
    static const uint16_t NVS_LAYOUT_VERSION_V1 = 1U;
    static const uint16_t NVS_LAYOUT_SIZE_BYTES_V1 = sizeof(NvsLayoutV1);

    /*
     * Increment this on every release where any configuration parameters has been changed. This
     * enforces a new NVS layout structure. Ensure also configuration migration strategy is defined! 
     */
    static const uint16_t CURRENT_LAYOUT_VERSION = NVS_LAYOUT_VERSION_V1;
    static const uint16_t CURRENT_LAYOUT_SIZE_BYTES = NVS_LAYOUT_SIZE_BYTES_V1;

    // Members
    bool initialized;
};

#endif // DEVICE_SETTINGS_H
#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include "Platform.h"
#include "ConfigurationLayout.h"

/**
 * @brief Represents the Non-Volatile storage for humi device. It uses
 * the underlying idf component which is based on key-value pair and uses
 * a portion of main flash memory through the esp_partition
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/storage/nvs_flash.html
 */
class DeviceStorage {
public:
    /**
     * @brief Construct a new Device Settings object
     */
    DeviceStorage();

    /**
     * @brief Destroy the Device Settings object
     */
    ~DeviceStorage();

    /**
     * @brief must be called before any r/w operation is executed.
     * 
     * @note Abort program if NVS can not be initialized!
     */
    void initialize();

    /**
     * @brief reset all configuration (except DeviceParameters) to factory defaults.
     */
    void factoryDefault();

    /**
     * @brief Get the Device Parameters object
     * 
     * @return DeviceParameters 
     */
    DeviceParameters getDeviceParameters(void);

    /**
     * @brief Set the Device Parameters object
     * 
     * @param param to set
     */
    void setDeviceParameters(const DeviceParameters& param);
    
private:
    /**
     * @brief Construct a new Device Settings object
     * 
     * @param copy instance
     */
    DeviceStorage(const DeviceStorage& copy);

    /**
     * @brief private assignment operator
     * 
     * @param other  instance
     * @return DeviceSettings& the assigned instance
     */
    DeviceStorage& operator=(const DeviceStorage& other);

    /**
     * @brief 
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
};

#endif // DEVICE_SETTINGS_H
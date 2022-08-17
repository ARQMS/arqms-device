#ifndef KEYVALUE_STORAGE_IFC_H_
#define KEYVALUE_STORAGE_IFC_H_

// Platform includes
#include <HumiDevice.Platform/Platform.h>

#define ESP_CTRL_PROP_DEVICE_SN             "SerialNumber"
#define ESP_CTRL_PROP_DEVICE_ROOM           "Room"
#define ESP_CTRL_PROP_DEVICE_INTERVAL       "Interval"
#define ESP_CTRL_PROP_DEVICE_BROKER_URI     "BrokerUri"
#define ESP_CTRL_PROP_WIFI_SSID             "Wifi_SSID"
#define ESP_CTRL_PROP_WIFI_PASSPHRASE       "Wifi_PWD"

/**
 * A very simple key-value storage driver. Can be used to get and set properties in storage.
 */
class KeyValueStorageIfc {
public:
    /**
     * Deconstructor
     */
    virtual ~KeyValueStorageIfc() { }

    /**
     * Reads the setting with given name from config provider.
     * 
     * @param name the name of setting to read
     * @param data the result.
     * @param size the size. Only required for dynamic sizes like strings; for primitive propertie types, the default value is used
     * @return esp_err_t 
     */
    virtual esp_err_t get(const char8_t* name, void** data, size_t* size) = 0;

    /**
     * writes the setting with given name to config provider.
     * 
     * @param name the name of setting to write
     * @param data the value.
     * @param size the size
     * @return esp_err_t 
     */
    virtual esp_err_t put(const char8_t* name, const void* data, const size_t size) = 0;
};

#endif // KEYVALUE_STORAGE_IFC_H_
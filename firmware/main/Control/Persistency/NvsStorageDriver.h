#ifndef DSDRIVER_H_
#define DSDRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "StorageDriverIfc.h"
#include "CloudLink/ConfigurationService/ConfigurationProviderIfc.h"

/**
 * This Storage Driver handles access to underlaying NVS architecture.
 */
class NvsStorageDriver : public StorageDriverIfc, public ConfigurationProviderIfc {
public:
    /**
     * Constructor
     */
    explicit NvsStorageDriver();

    /**
     * Destructor
     */
    ~NvsStorageDriver();

    /**
     * Initialize NVS storage
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
     * @see ConfigurationProviderIfc::readConfiguration
     */
    virtual esp_err_t readConfiguration(const char8_t* name, void** data, size_t* size) override;

    /**
     * @see ConfigurationProviderIfc::writeConfiguration
     */
    virtual esp_err_t writeConfiguration(const char8_t* name, const void* data, const size_t size) override;

    /**
     * @see  StorageDriverIfc::readWifiConfig
     * 
     * @param pWifiParam 
     */
    virtual void readWifiConfig(WifiSettingsEvent* pWifiParam) override;

    /**
     * @see StorageDriverIfc::readDeviceConfig
     */
    virtual void readDeviceConfig(DeviceSettingsEvent* pDeviceParam) override;
    
private:
    // Change this version on each layout change!!
    const static uint8_t LAYOUT_VERSION = 1;

    // Helper Method
    template<typename T>
    esp_err_t storeData(SerializableIfc& data, const char8_t* key) {
        static uint8_t binaryBlob[sizeof(T)];
        return storeData(data, key, binaryBlob, sizeof(T));
    }
    esp_err_t storeData(SerializableIfc& data, const char8_t* key, uint8_t* buffer, const size_t size);

    template<typename T>
    esp_err_t loadData(SerializableIfc& data, const char8_t* key) const {
        static uint8_t binaryBlob[sizeof(T)];
        size_t size = sizeof(T);
        return loadData(data, key, binaryBlob, &size);
    }
    esp_err_t loadData(SerializableIfc& data, const char8_t* key, uint8_t* buffer, size_t* size) const;

    bool checkVersion() const;
    bool checkEspError(const esp_err_t erroCode) const;

    // Members
    const static char8_t* s_partitionNamespace;
    const static char8_t* s_wifiConfigKey;
    const static char8_t* s_deviceConfigKey;
    const static char8_t* s_versionKey;

};


#endif //DSDRIVER_H_

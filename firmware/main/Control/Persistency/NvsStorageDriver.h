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
    const static char8_t* partitionNamespace;
    const static char8_t* wifiConfigKey;
    const static char8_t* deviceConfigKey;
};


#endif //DSDRIVER_H_

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
     * @see ConfigurationProviderIfc::readConfiguration
     */
    virtual esp_err_t readConfiguration(const char* name, void** data, size_t* size) override;

    /**
     * @see ConfigurationProviderIfc::writeConfiguration
     */
    virtual esp_err_t writeConfiguration(const char* name, const void* data, const size_t size) override;

    /**
     * @see StorageDriverIfc::readWifiConfig
     */
    virtual void readWifiConfig(WifiParameters* pWifiParam) override;
};


#endif //DSDRIVER_H_
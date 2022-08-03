#ifndef DSDRIVER_H_
#define DSDRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "StorageDriverIfc.h"
#include "CloudLink/ConfigProviderIfc.h"

/**
 * This Storage Driver handles access to underlaying NVS architecture.
 */
class NvsStorageDriver : public StorageDriverIfc, public ConfigProviderIfc {
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
     * @see ConfigProviderIfc::readConfiguration
     */
    virtual esp_err_t readConfiguration(const char* name, void** data, size_t* size) override;

    /**
     * @see ConfigProviderIfc::writeConfiguration
     */
    virtual esp_err_t writeConfiguration(const char* name, void* data, size_t size) override;
};


#endif //DSDRIVER_H_
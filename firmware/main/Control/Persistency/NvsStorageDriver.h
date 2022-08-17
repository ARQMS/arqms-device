#ifndef DSDRIVER_H_
#define DSDRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "StorageDriverIfc.h"
#include "NvsLayout.h"
#include "CloudLink/ConfigurationService/KeyValueStorageIfc.h"

/**
 * This Storage Driver handles access to underlaying NVS architecture.
 */
class NvsStorageDriver : public StorageDriverIfc, public KeyValueStorageIfc {
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
     * @see KeyValueStorageIfc::get
     */
    virtual esp_err_t get(const char8_t* name, void** data, size_t* size) override;

    /**
     * @see KeyValueStorageIfc::put
     */
    virtual esp_err_t put(const char8_t* name, const void* data, const size_t size) override;

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
    // Helper Method
    bool checkVersion();

    NvsLayout m_layout;
};


#endif //DSDRIVER_H_

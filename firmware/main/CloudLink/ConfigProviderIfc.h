#ifndef CONFIGPROVIDER_IFC_H_
#define CONFIGPROVIDER_IFC_H_

// Platform includes
#include <HumiDevice.Platform/Platform.h>

// IDF includes
#include <esp_local_ctrl.h>

/**
 * A configuration provider to get and set configurations
 */
class ConfigProviderIfc {
public:
    /**
     * Deconstructor
     */
    virtual ~ConfigProviderIfc() { }
};

#endif // CONFIGPROVIDER_IFC_H_
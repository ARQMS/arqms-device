#ifndef CONFIGURATION_SERVICE_IFC_H_
#define CONFIGURATION_SERVICE_IFC_H_

// Platform includes
#include <HumiDevice.Platform/Platform.h>

/**
 * Used for local ctrl actions to get and set configurations.
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/protocols/esp_local_ctrl.html
 */
class ConfigurationServiceIfc {
public:
    /**
     * Deconstructor
     */
    virtual ~ConfigurationServiceIfc() { }

    /**
     * Starts the ctrl service
     */
    virtual esp_err_t startService() = 0;

    /**
     * Stops the ctrl service
     */
    virtual esp_err_t stopService() = 0;
};

#endif // CONFIGURATION_SERVICE_IFC_H_
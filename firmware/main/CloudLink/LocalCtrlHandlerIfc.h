#ifndef LOCALCTRLHANDLER_IFC_H
#define LOCALCTRLHANDLER_IFC_H

// Platform includes
#include <HumiDevice.Platform/Platform.h>


/**
 * Used for local ctrl actions to get and set configurations.
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/protocols/esp_local_ctrl.html
 */
class LocalCtrlHandlerIfc {
public:
    /**
     * Deconstructor
     */
    virtual ~LocalCtrlHandlerIfc() { }

    /**
     * Starts the ctrl service
     */
    virtual esp_err_t startService() = 0;

    /**
     * Stops the ctrl service
     */
    virtual esp_err_t stopService() = 0;
};

#endif // LOCALCTRLHANDLER_IFC_H
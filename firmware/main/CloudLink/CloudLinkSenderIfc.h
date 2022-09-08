#ifndef WIFI_SM_IFC_H_
#define WIFI_SM_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "Events/WifiStatusEvent.h"

/**
 * Represents the state machine callback interface
 */
class CloudLinkSenderIfc {
public:

    /**
     * Destructor
     */
    virtual ~CloudLinkSenderIfc() {};

    /**
     * Sends a wifi status event to callback
     * 
     * @param status the current status
     */
    virtual void sendWifiStatus(const WifiStatus status) = 0;
};


#endif // WIFI_SM_IFC_H_
#ifndef WIFI_SM_IFC_H_
#define WIFI_SM_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "Events/WifiStatusEvent.h"

/**
 * Represents the state machine callback interface
 */
class WifiStateMachineIfc {
public:

    /**
     * Destructor
     */
    virtual ~WifiStateMachineIfc() {};

    /**
     * 
     */
    virtual void sendWifiStatus(const WifiStatus status, const int32_t rssi = 0) = 0;
};


#endif // WIFI_SM_IFC_H_
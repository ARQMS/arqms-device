#ifndef CORE_SM_IFC_H_
#define CORE_SM_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "Events/WifiSettingsEvent.h"

/**
 * Represents the state machine callback interface
 */
class CoreSMIfc {
public:

    /**
     * Destructor
     */
    virtual ~CoreSMIfc() {};

    /**
     * Sends a wifi event
     * 
     * @param wifiEvent to send
     */
    virtual void sendWifi(WifiSettingsEvent& wifiEvent) = 0;
};


#endif // CORE_SM_IFC_H_
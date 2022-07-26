#ifndef WIFI_SM_IFC_H_
#define WIFI_SM_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

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
    virtual void onClientConnected() = 0;

    /**
     * 
     */
    virtual void onClientDisconnected() = 0;

    /**
     * 
     */
    virtual void onServiceModeIdle() = 0;

    /**
     * 
     */
    virtual void onNormalModeIdle() = 0;
};


#endif // WIFI_SM_IFC_H_
#ifndef DSDRIVER_IFC_H_
#define DSDRIVER_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "Events/DeviceInfoEvent.h"
#include "Events/WifiSettingsEvent.h"

/**
 * Storage driver provides access to configuration
 */
class StorageDriverIfc {
public:

    /**
     * Destructor
     */
    virtual ~StorageDriverIfc() {};

    /**
     * Gets the wifi configuration from underlaying storage
     * 
     * @param const WifiParameters* 
     */
    virtual void readWifiConfig(WifiSettingsEvent* pWifiParam) = 0;

    /**
     * Gets the device configuration from underlaying storage
     * 
     * @param const DeviceParameters* 
     */
    virtual void readDeviceConfig(DeviceInfoEvent* pDeviceParam) = 0;

    /**
     * Resets underlaying storage to default values. This action can not be undo!
     */
    virtual void reset() = 0;
};


#endif //DSDRIVER_IFC_H_
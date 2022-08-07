#ifndef DSDRIVER_IFC_H_
#define DSDRIVER_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "Control/Persistency/NvsStorageLayout.h"

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
    virtual void readWifiConfig(WifiParameters* pWifiParam) = 0;
};


#endif //DSDRIVER_IFC_H_
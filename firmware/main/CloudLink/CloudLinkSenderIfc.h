#ifndef WIFI_SM_IFC_H_
#define WIFI_SM_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "Events/WifiStatusEvent.h"
#include "MqttService/HDP/HDP.h"

// represents an invalid rssi
const static int32_t INVALID_RSSI = -127;

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
     * @param rssi (optional) signal strength. 0 means not connected
     */
    virtual void sendWifiStatus(const WifiStatus status, int32_t rssi = INVALID_RSSI) = 0;

    /**
     * Fired when device configuration received
     * 
     * @param msg the received message
     */
    virtual void onDeviceConfig(const HDPDeviceConfig& msg) = 0;

    /**
     * Fired when update info received
     * 
     * @param msg the received message
     */
    virtual void onUpdateInfo(const HDPUpdateInfo& msg) = 0;
};


#endif // WIFI_SM_IFC_H_
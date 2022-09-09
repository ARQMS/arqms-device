#ifndef HDP_DEVICE_STATUS_H_
#define HDP_DEVICE_STATUS_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "HDPMessage.h"

/**
 * Immutable device status object for HDP.
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/HumiDevice-Protocol
 */
class HDPDeviceStatus : public HDPMessage {
public:
    /**
     * Constructor
     */
    HDPDeviceStatus(const float32_t batteryLevel,
                    const float32_t wifiRssi,
                    const uint32_t runningTime) :
        m_batteryLevel(batteryLevel),
        m_wifiRssi(wifiRssi),
        m_runningTime(runningTime) {
    };

    /**
     * Destructor
     */
    virtual ~HDPDeviceStatus() {
    };

    /**
     * @see HDPMessage::getJson
     */
    virtual void getJson(cJSON* obj) const override {
        cJSON_AddNumberToObject(obj, "BatteryLevel", m_batteryLevel);
        cJSON_AddNumberToObject(obj, "WifiRssi", m_wifiRssi);
        cJSON_AddNumberToObject(obj, "RunningTime", m_runningTime);
    };

private:
    const float32_t m_batteryLevel;
    const float32_t m_wifiRssi;
    const uint32_t m_runningTime;
};

#endif // HDP_DEVICE_STATUS_H_
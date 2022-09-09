#ifndef HDP_DEVICE_CONFIG_H_
#define HDP_DEVICE_CONFIG_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "HDPMessage.h"

/**
 * Immutable device config object for HDP.
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/HumiDevice-Protocol
 */
class HDPDeviceConfig : public HDPMessage {
public:
    // constants
    const static uint32_t MAX_CHANNEL_LENGTH = 32;

    /**
     * Constructor
     */
    HDPDeviceConfig() {
    };

    /**
     * Destructor
     */
    virtual ~HDPDeviceConfig() {
    };

    /**
     * Setter for channel
     * 
     * @param channel to set, NULL will clear channel
     */
    void setChannel(const char8_t channel[MAX_CHANNEL_LENGTH]) {
        if (channel != NULL) {
            memcpy(m_channel, channel, MAX_CHANNEL_LENGTH);
        }
        else {
            memset(m_channel, 0, MAX_CHANNEL_LENGTH);
        }
    }

    /**
     * Getter for channel
     * 
     * @param channel to get
     */
    void getChannel(char8_t channel[MAX_CHANNEL_LENGTH]) const {
        if (channel != NULL) {
            memcpy(channel, m_channel, MAX_CHANNEL_LENGTH);
        }
    }

    /**
     * Setter for interval 
     * 
     * @param interval to set in sec
     */
    void setInterval(const uint32_t interval) {
        m_interval = interval;
    }

    /**
     * Getter for interval
     * 
     * @return uint32_t the interval in sec
     */
    uint32_t getInterval() const {
        return m_interval;
    }

    /**
     * @see HDPMessage::getJson
     */
    virtual void getJson(cJSON* obj) const override {
        cJSON_AddStringToObject(obj, "Channel", m_channel);
        cJSON_AddNumberToObject(obj, "Interval", m_interval);
    };

private:
    char8_t m_channel[MAX_CHANNEL_LENGTH];
    uint32_t m_interval;
};

#endif // HDP_DEVICE_CONFIG_H_
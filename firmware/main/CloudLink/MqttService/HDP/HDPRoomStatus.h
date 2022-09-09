#ifndef HDP_ROOM_STATUS_H_
#define HDP_ROOM_STATUS_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "HDPMessage.h"

/**
 * Immutable room info object for HDP.
 */
class HDPRoomStatus : public HDPMessage {
public:
    /**
     * Constructor
     */
    HDPRoomStatus(const float32_t humidity,
                  const float32_t pressure,
                  const float32_t temperature,
                  const float32_t gasResistance) :
        m_humidity(humidity),
        m_pressure(pressure),
        m_temperature(temperature),
        m_gasResistance(gasResistance) {
    };

    /**
     * Destructor
     */
    virtual ~HDPRoomStatus() {
    };

    /**
     * @see HDPMessage::getJson
     */
    virtual void getJson(cJSON* obj) const override {
        cJSON_AddNumberToObject(obj, "Humidity", m_humidity);
        cJSON_AddNumberToObject(obj, "Pressure", m_pressure);
        cJSON_AddNumberToObject(obj, "Temperature", m_temperature);
        cJSON_AddNumberToObject(obj, "GasResistance", m_gasResistance);
    };

private:
    const float32_t m_humidity;
    const float32_t m_pressure;
    const float32_t m_temperature;
    const float32_t m_gasResistance;
};

#endif // HDP_ROOM_STATUS_H_
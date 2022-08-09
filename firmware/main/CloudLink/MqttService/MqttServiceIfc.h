#ifndef MQTT_SERVICE_IFC_H_
#define MQTT_SERVICE_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "Events/DeviceSettingsEvent.h"

/**
 * MQTT handler to publish and receive any supported mqtt packages
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/protocols/mqtt.html
 */
class MqttServiceIfc {
public:
    /**
     * Deconstructor
     */
    virtual ~MqttServiceIfc() { };

    /**
     * Starts the mqtt service
     * 
     * @param deviceSettings settings of the device for mqtt
     */
    virtual esp_err_t startService(const DeviceSettingsEvent& deviceSettings) = 0;

    /**
     * Stops the mqtt service
     */
    virtual esp_err_t stopService(void) = 0;
};

#endif // MQTT_SERVICE_IFC_H_
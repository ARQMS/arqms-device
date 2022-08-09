#ifndef MqttService_H
#define MqttService_H

// Platform include
#include <HumiDevice.Platform/Platform.h>

// idf includes
#include "mqtt_client.h"

// Project includes
#include "MqttServiceIfc.h"

/**
 * MQTT handler to publish and receive any supported mqtt packages
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/protocols/mqtt.html
 */
class MqttService : public MqttServiceIfc {
public:
    /**
     * Constructor
     */
    explicit MqttService();

    /**
     * Deconstructor
     */
    virtual ~MqttService() { }

    /**
     * @see MqttServiceIfc::startService
     */
    virtual esp_err_t startService(const DeviceSettingsEvent& deviceSettings) override;

    /**
     * @see MqttServiceIfc::stopService
     */
    virtual esp_err_t stopService(void) override;

private:
    /**
     * Private copy constructor.
     */
    MqttService(const MqttService& other);

    /**
     * Private assignment operator.
     */
    MqttService& operator=(const MqttService& other);

    /**
     * Is called when device is successfully connected to MQTT Broker.
     * Just ensure you subscribe to interesting channels
     */
    void onConnected();

    /**
     * Is called when an event is received on subscribed channels
     * 
     * @param event data
     */
    void onMqttReceived(const esp_mqtt_event_handle_t event);

    /**
     * Event handler registered to receive MQTT events
     *
     *  This function is called by the MQTT client event loop.
     *
     * @param handler_args user data registered to the event.
     * @param base Event base for the handler(always MQTT Base in this example).
     * @param event_id The id for the received event.
     * @param event_data The data for the event, esp_mqtt_event_handle_t.
     */
    static void mqttEventHandler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

    /**
     * Qos state for events
     * @see https://www.hivemq.com/blog/mqtt-essentials-part-6-mqtt-quality-of-service-levels/
     */
    enum MqttQoS {
        AT_MOST_ONCE = 0,
        AT_LEAST_ONCE = 1,
        EXACTLY_INCE = 2,
    };

    // Members
    esp_mqtt_client_handle_t m_mqttClient;
};

#endif // MqttService_H
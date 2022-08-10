#ifndef MqttService_H
#define MqttService_H

// Platform include
#include <HumiDevice.Platform/Platform.h>

// idf includes
#include "mqtt_client.h"

// Project includes
#include "MqttServiceIfc.h"
#include "CloudLink/CloudLinkSenderIfc.h"
#include "Events/SensorDataEvent.h"

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
    explicit MqttService(CloudLinkSenderIfc& sender);

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

    /**
     * Publishes the sensor event data
     * 
     * @param data to publish
     */
    void publish(const SensorDataEvent& data);

private:
    /**
     * Private copy constructor.
     */
    MqttService(const MqttService& other);

    /**
     * Private assignment operator.
     */
    MqttService& operator=(const MqttService& other);

    // Helper
    void onConnected();
    void onDisconnected();
    void onFailure(const esp_mqtt_event_handle_t event);
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

    // helper method
    void publish(const char8_t* topic, const float32_t data);

    // Members
    esp_mqtt_client_handle_t m_pMqttClient;
    CloudLinkSenderIfc& m_sender;
};

#endif // MqttService_H
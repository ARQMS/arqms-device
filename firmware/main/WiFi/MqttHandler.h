#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include "HumiPlatform.h"
#include "DeviceStorage/DeviceStorage.h"
#include "Events/EventLoopIfc.h"
#include "Events/EventHandlerIfc.h"

#include "mqtt_client.h"

/**
 * MQTT handler to publish and receive any supported mqtt packages
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/protocols/mqtt.html
 */
class MqttHandler : public EventHandlerIfc {
public:
    /**
     * Constructor
     */
    explicit MqttHandler(DeviceStorage& storage, EventLoopIfc& eventLoop);

    /**
     * Deconstructor
     */
    ~MqttHandler() { }

    /**
     * Starts the mqtt service
     * 
     * @param brokerUri the null-terminated broker uri
     */
    void startService(const char* brokerUri);

    /**
     * Stops the mqtt service
     */
    void stopService(void);

    /**
     * Indicates when local mqtt service is running
     *
     * @return bool true when service is already running, otherwise false.
     */
    bool isRunning() const;
    
    /**
     * @see EventHandlerIfc
     */
    void onEvent(esp_event_base_t base, int32_t id, void* data) override;
private:
    /**
     * Private copy constructor.
     */
    MqttHandler(const MqttHandler& other);

    /**
     * Private assignment operator.
     */
    MqttHandler& operator=(const MqttHandler& other);

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
    bool serviceEnabled;
    esp_mqtt_client_handle_t client;
    DeviceStorage& storage;
    EventLoopIfc& eventLoop;
};

#endif // MQTTHANDLER_H
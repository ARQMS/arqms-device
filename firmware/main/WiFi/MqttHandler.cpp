#include "MqttHandler.h"

#include "esp_event.h"
#include "esp_tls.h"
#include "Events/Events.h"

#define TAG "MqttHandler"

// MQTT Channels
#define MQTT_SUB_NEW_FIRMWARE "/devices/#/updates"
#define MQTT_SUB_CFG          "/devices/development/cfg"

extern const uint8_t mqtt_pem_start[] asm("_binary_mqtt_pem_start");
extern const uint8_t mqtt_pem_end[]   asm("_binary_mqtt_pem_end");

MqttHandler::MqttHandler(DeviceStorage& storage, EventLoopIfc& eventLoop) :
    serviceEnabled(false),
    client(),
    storage(storage),
    eventLoop(eventLoop) {
}

void MqttHandler::startService(const char* brokerUri) {
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = brokerUri,
        .cert_pem = (const char*)mqtt_pem_start,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, &MqttHandler::mqttEventHandler, this);
    const esp_err_t err = esp_mqtt_client_start(client);
    serviceEnabled = err == ERR_OK;
}

void MqttHandler::stopService() {
    serviceEnabled = false;
}

bool MqttHandler::isRunning() const {
    return serviceEnabled;
}

void MqttHandler::onEvent(esp_event_base_t base, int32_t id, void* data) {
    if (base == SENSOR_EVENTS && id == DATA_ACQUIRED) { 
        // SensorData* pSensorData = static_cast<SensorData*>(data);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/humidity", pSensorData->relativeHumidity, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/pressure", pSensorData->pressure, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/temperature", pSensorData->temperature, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/voc", pSensorData->voc, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/co2", pSensorData->co2, sizeof(float32_t), AT_MOST_ONCE, false);
    }
}

void MqttHandler::onConnected() {
    esp_mqtt_client_subscribe(client, MQTT_SUB_NEW_FIRMWARE, AT_LEAST_ONCE);
    esp_mqtt_client_subscribe(client, MQTT_SUB_CFG, AT_LEAST_ONCE);
}

void MqttHandler::onMqttReceived(const esp_mqtt_event_handle_t event) {
    if (strcmp(event->topic, MQTT_SUB_NEW_FIRMWARE)) {
        DeviceFirmwareInfo info = {};
        eventLoop.sendEvent(DEVICE_EVENTS, AppEvents::NEW_FIRMWARE_FOUND, &info, sizeof(DeviceFirmwareInfo));
    }
}

void MqttHandler::mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    MqttHandler* pHandler = static_cast<MqttHandler*>(handler_args);

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED: {
                pHandler->onConnected();
            }
            break;

        case MQTT_EVENT_DATA: {
                esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
                pHandler->onMqttReceived(event);
            }
            break;

        case MQTT_EVENT_ERROR:
            // TODO
            break;

        default:
            break;
    }
}

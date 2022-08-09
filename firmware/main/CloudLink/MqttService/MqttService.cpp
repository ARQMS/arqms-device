#include "MqttService.h"

#include "esp_tls.h"

// MQTT Channels
#define MQTT_SUB_NEW_FIRMWARE "/devices/#/updates"
#define MQTT_SUB_CFG          "/devices/development/cfg"

// extern const uint8_t mqtt_pem_start[] asm("_binary_mqtt_pem_start");
// extern const uint8_t mqtt_pem_end[]   asm("_binary_mqtt_pem_end");

MqttService::MqttService() :
    m_mqttClient() {
}

esp_err_t MqttService::startService(const DeviceSettingsEvent& deviceSettings) {
    char8_t brokerUri[DeviceSettingsEvent::MAX_BROKER_URI_LENGTH];
    deviceSettings.getBrokerUri(brokerUri);

    char8_t sn[DeviceSettingsEvent::MAX_SN_LENGTH];
    deviceSettings.getSn(sn);

    const esp_mqtt_client_config_t mqtt_cfg = {
       .uri = brokerUri,
       .client_id = sn
    };

    m_mqttClient = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(m_mqttClient, MQTT_EVENT_ANY, &MqttService::mqttEventHandler, this);
    return esp_mqtt_client_start(m_mqttClient);
}

esp_err_t MqttService::stopService() {
    return esp_mqtt_client_stop(m_mqttClient);
}
/*
void MqttService::onEvent(esp_event_base_t base, int32_t id, void* data) {
    if (base == SENSOR_EVENTS && id == DATA_ACQUIRED) { 
        // SensorData* pSensorData = static_cast<SensorData*>(data);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/humidity", pSensorData->relativeHumidity, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/pressure", pSensorData->pressure, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/temperature", pSensorData->temperature, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/voc", pSensorData->voc, sizeof(float32_t), AT_MOST_ONCE, false);
        // esp_mqtt_client_publish(client, "/device/<mysn>/room/co2", pSensorData->co2, sizeof(float32_t), AT_MOST_ONCE, false);
    }
}*/

void MqttService::onConnected() {
    esp_mqtt_client_subscribe(m_mqttClient, MQTT_SUB_NEW_FIRMWARE, AT_LEAST_ONCE);
    esp_mqtt_client_subscribe(m_mqttClient, MQTT_SUB_CFG, AT_LEAST_ONCE);
}

void MqttService::onMqttReceived(const esp_mqtt_event_handle_t event) {
    ESP_LOGW("HumiMqtt", "received topic %s", event->topic);

    if (strcmp(event->topic, MQTT_SUB_NEW_FIRMWARE)) {
        // DeviceFirmwareInfo info = {};
        //eventLoop.sendEvent(DEVICE_EVENTS, AppEvents::NEW_FIRMWARE_FOUND, &info, sizeof(DeviceFirmwareInfo));
    }
}

void MqttService::mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    MqttService* pHandler = static_cast<MqttService*>(handler_args);

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            pHandler->onConnected();
            break;

        case MQTT_EVENT_DATA: {
                esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
                pHandler->onMqttReceived(event);
            }
            break;

        case MQTT_EVENT_DISCONNECTED:
        case MQTT_EVENT_ERROR:
            ESP_LOGE("HumiMqtt", "Error or disconnected");
            // TODO
            break;

        default:
            break;
    }
}

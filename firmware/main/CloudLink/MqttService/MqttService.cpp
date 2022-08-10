#include "MqttService.h"

#include "MqttUtil.h"

MqttService::MqttService() :
    m_pMqttClient(NULL) {
}

esp_err_t MqttService::startService(const DeviceSettingsEvent& deviceSettings) {
    char8_t brokerUri[DeviceSettingsEvent::MAX_BROKER_URI_LENGTH];
    char8_t sn[DeviceSettingsEvent::MAX_SN_LENGTH];

    deviceSettings.getBrokerUri(brokerUri);
    deviceSettings.getSn(sn);

    const esp_mqtt_client_config_t mqtt_cfg = {
       .uri = brokerUri,
       .client_id = sn
    };

    // register possible placeholders, just ensure MqttUtil::NUM_PLACEHOLDER matches!
    MqttUtil::registerPlaceholder("$sn", sn);
    MqttUtil::registerPlaceholder("$channel", "dev");

    m_pMqttClient = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(m_pMqttClient, MQTT_EVENT_ANY, &MqttService::mqttEventHandler, this);
    return esp_mqtt_client_start(m_pMqttClient);
}

esp_err_t MqttService::stopService() {
    return esp_mqtt_client_stop(m_pMqttClient);
}

void MqttService::publish(const SensorDataEvent& data) {
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/humidity", data.getRelativeHumidity());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/pressure", data.getPressure());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/temperature", data.getTemperature());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/voc", data.getVoc());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/co2", data.getCo2());
}

void MqttService::onConnected() {
    MqttUtil::subscribe(m_pMqttClient, "devices/$channel/update");
    MqttUtil::subscribe(m_pMqttClient, "devices/$sn/config");
}

void MqttService::onMqttReceived(const esp_mqtt_event_handle_t event) {
    ESP_LOGD("Humi", "Received %.*s", event->topic_len, event->topic);

    // Demo
    SensorDataEvent msg(1.2, 98.8, 20.5, 12.2, 0.5);
    publish(msg);

    // TODO handle new firmware, Attention: update has a placehlder 'device/$sn/config
    // if (strcmp(event->topic, MQTT_SUB_DEVICE_CONFIGURATION)) {
    // }

    // TODO handle new firmware, Attention: update has a placehlder 'device/$channel/update
    // if (strcmp(event->topic, MQTT_SUB_NEW_FIRMWARE)) {
    // }
}

void MqttService::mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    MqttService* pHandler = static_cast<MqttService*>(handler_args);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            pHandler->onConnected();
            break;

        case MQTT_EVENT_DATA: {
                esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
                pHandler->onMqttReceived(event);
            }
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE("HumiMqtt", "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                ESP_LOGE("HumiMqtt", "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;

        default:
            break;
    }
}

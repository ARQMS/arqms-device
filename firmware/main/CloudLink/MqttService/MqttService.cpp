#include "MqttService.h"

#include "MqttUtil.h"

MqttService::MqttService(CloudLinkSenderIfc& sender) :
    m_pMqttClient(NULL),
    m_sender(sender) {
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
    m_sender.sendWifiStatus(WifiStatus::MQTT_SENDING);

    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/humidity", data.getRelativeHumidity());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/pressure", data.getPressure());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/temperature", data.getTemperature());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/voc", data.getVoc());
    MqttUtil::publish(m_pMqttClient, "devices/$sn/room/co2", data.getCo2());

    m_sender.sendWifiStatus(WifiStatus::MQTT_SENDED);
}

void MqttService::onConnected() {
    // Register subscriptions
    MqttUtil::subscribe(m_pMqttClient, "devices/$channel/update");
    MqttUtil::subscribe(m_pMqttClient, "devices/$sn/config");

    m_sender.sendWifiStatus(WifiStatus::MQTT_CONNECTED);

    // TODO, remove demo
    MqttUtil::publish(m_pMqttClient, "devices/$sn/status", "Device online: But data not decoded");
}

void MqttService::onDisconnected() {
    m_sender.sendWifiStatus(WifiStatus::MQTT_DISCONNECTED);
}

void MqttService::onFailure(const esp_mqtt_event_handle_t event) {
    m_sender.sendWifiStatus(WifiStatus::UNKNOWN_ERROR);
}

void MqttService::onMqttReceived(const esp_mqtt_event_handle_t event) {
    m_sender.sendWifiStatus(WifiStatus::MQTT_RECEIVED);

    ESP_LOGI("Humi", "Received %.*s", event->topic_len, event->topic);

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

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            pHandler->onConnected();
            break;

        case MQTT_EVENT_DISCONNECTED:
            pHandler->onDisconnected();
            break;

        case MQTT_EVENT_DATA: 
            pHandler->onMqttReceived(static_cast<esp_mqtt_event_handle_t>(event_data));
            break;

        case MQTT_EVENT_ERROR:
            pHandler->onFailure(static_cast<esp_mqtt_event_handle_t>(event_data));
            break;

        default:
            break;
    }
}

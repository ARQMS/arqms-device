#include "MqttService.h"

#include "MqttUtil.h"
#include <cJSON.h>

#define TOPIC_UPDATE_INFO "devices/update/$channel"
#define TOPIC_CONFIG      "devices/$sn/config"

MqttService::MqttService(CloudLinkSenderIfc& sender) :
    m_pMqttClient(NULL),
    m_sender(sender) {
}

esp_err_t MqttService::startService(const DeviceInfoEvent& deviceSettings) {
    char8_t brokerUri[DeviceInfoEvent::MAX_BROKER_URI_LENGTH];
    char8_t sn[DeviceInfoEvent::MAX_SN_LENGTH];
    char8_t channel[DeviceInfoEvent::MAX_CHANNEL_LENGTH];

    deviceSettings.getBrokerUri(brokerUri);
    deviceSettings.getSn(sn);
    deviceSettings.getChannel(channel);

    // just ensure broker is configured
    if (strlen(brokerUri) <= 0 || strlen(channel) <= 0 || strlen(sn) <= 0) {
        return ESP_ERR_INVALID_ARG;
    }

    const esp_mqtt_client_config_t mqtt_cfg = {
       .uri = brokerUri,
       .client_id = sn
    };

    // register possible placeholders, just ensure MqttUtil::NUM_PLACEHOLDER matches and length is not longer than MqttUtil::MAX_PLACEHOLDER_LENGTH
    MqttUtil::registerPlaceholder("$sn", sn);
    MqttUtil::registerPlaceholder("$channel", channel);

    m_pMqttClient = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(m_pMqttClient, MQTT_EVENT_ANY, &MqttService::mqttEventHandler, this);

    return esp_mqtt_client_start(m_pMqttClient);
}

esp_err_t MqttService::stopService() {
    return esp_mqtt_client_stop(m_pMqttClient);
}

void MqttService::publish(const char8_t* topic, const HDPMessage& data) {
    cJSON* obj = cJSON_CreateObject();

    data.getJson(obj);

    char8_t* json = cJSON_Print(obj);
    MqttUtil::publish(m_pMqttClient, topic, json);

    cJSON_free(json);
    cJSON_Delete(obj);
}

void MqttService::onConnected() {
    // Register subscriptions
    MqttUtil::subscribe(m_pMqttClient, TOPIC_UPDATE_INFO);
    MqttUtil::subscribe(m_pMqttClient, TOPIC_CONFIG);

    m_sender.sendWifiStatus(WifiStatus::MQTT_CONNECTED);
}

void MqttService::onDisconnected() {
    m_sender.sendWifiStatus(WifiStatus::MQTT_DISCONNECTED);
}

void MqttService::onFailure(const esp_mqtt_event_handle_t event) {
    m_sender.sendWifiStatus(WifiStatus::UNKNOWN_ERROR);
}

void MqttService::onMqttReceived(const esp_mqtt_event_handle_t event) {
    // do not parse event without any content
    if (event->data_len <= 0) return;

    cJSON* obj = cJSON_Parse(event->data);

    if (MqttUtil::isTopic(TOPIC_CONFIG, event->topic)) {
        m_sender.onDeviceConfig(HDPDeviceConfig(obj));
    } 
    else if (MqttUtil::isTopic(TOPIC_UPDATE_INFO, event->topic)) {
        m_sender.onUpdateInfo(HDPUpdateInfo(obj));
    }

    cJSON_Delete(obj);
    
    // we have received message once. so we clear retained message https://stackoverflow.com/a/36729560/7110375
    if (event->retain) {
        esp_mqtt_client_publish(m_pMqttClient, event->topic, 0, 0, AT_LEAST_ONCE, true);
    }
}

void MqttService::mqttEventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data) {
    MqttService* pHandler = static_cast<MqttService*>(handler_args);
    esp_mqtt_event_handle_t handle = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            pHandler->onConnected();
            break;

        case MQTT_EVENT_DISCONNECTED:
            pHandler->onDisconnected();
            break;

        case MQTT_EVENT_DATA: 
            pHandler->onMqttReceived(handle);
            break;

        case MQTT_EVENT_ERROR: 
            // ignore TSL and TCP errors
            if (handle->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
                pHandler->onFailure(handle);
            }
            break;

        default:
            break;
    }
}

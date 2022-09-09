#include "MqttService.h"

#include "MqttUtil.h"
#include <cJSON.h>

MqttService::MqttService(CloudLinkSenderIfc& sender) :
    m_pMqttClient(NULL),
    m_sender(sender) {
}

esp_err_t MqttService::startService(const DeviceSettingsEvent& deviceSettings) {
    char8_t brokerUri[DeviceSettingsEvent::MAX_BROKER_URI_LENGTH];
    char8_t sn[DeviceSettingsEvent::MAX_SN_LENGTH];

    deviceSettings.getBrokerUri(brokerUri);
    deviceSettings.getSn(sn);
    // TODO get channel

    // just ensure broker is configured
    if (strlen(brokerUri) <= 0) {
        return ESP_ERR_INVALID_ARG;
    }

    const esp_mqtt_client_config_t mqtt_cfg = {
       .uri = brokerUri,
       .client_id = sn
    };

    // register possible placeholders, just ensure MqttUtil::NUM_PLACEHOLDER matches and length is not longer than MqttUtil::MAX_PLACEHOLDER_LENGTH
    MqttUtil::registerPlaceholder("$sn", sn);
    MqttUtil::registerPlaceholder("$channel", "DEV");

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
    MqttUtil::subscribe(m_pMqttClient, "devices/update/$channel");
    MqttUtil::subscribe(m_pMqttClient, "devices/$sn/config");

    m_sender.sendWifiStatus(WifiStatus::MQTT_CONNECTED);
}

void MqttService::onDisconnected() {
    m_sender.sendWifiStatus(WifiStatus::MQTT_DISCONNECTED);
}

void MqttService::onFailure(const esp_mqtt_event_handle_t event) {
    m_sender.sendWifiStatus(WifiStatus::UNKNOWN_ERROR);
}

void MqttService::onMqttReceived(const esp_mqtt_event_handle_t event) {
    cJSON *obj = cJSON_Parse(event->data);
    cJSON* item = NULL;

    if (MqttUtil::isTopic("devices/$sn/config", event->topic)) {
        HDPDeviceConfig deviceConfig;

        item = cJSON_GetObjectItem(obj, "Channel");
        if (item != NULL) {
            deviceConfig.setChannel(item->valuestring);
        }

        item = cJSON_GetObjectItem(obj, "Interval");
        if (item != NULL) {
            deviceConfig.setInterval(item->valueint);
        }

        m_sender.onDeviceConfig(deviceConfig);
    } 
    else if (MqttUtil::isTopic("devices/$channel/update", event->topic)) {
        HDPUpdateInfo updateInfo;

        item = cJSON_GetObjectItem(obj, "DownloadURI");
        if (item != NULL) {
            updateInfo.setDownloadUri(item->valuestring);
        }

        item = cJSON_GetObjectItem(obj, "Version");
        if (item != NULL) {
            updateInfo.setVersion(item->valueint);
        }

        m_sender.onUpdateInfo(updateInfo);
    }

    cJSON_Delete(obj);
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

#include "CloudLinkTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiStatusEvent.h"
#include "MqttService/HDP/HDP.h"

CloudLinkTask::CloudLinkTask() :
    StatusEvent(),
    m_ctrlHandler(),
    m_mqttService(*this),
    m_wifi(*this, m_ctrlHandler, m_mqttService),
    m_lastWifiEvent(),
    m_pTimeoutTimer(NULL),
    m_pWifiStateUpdater(NULL) {
}

CloudLinkTask::~CloudLinkTask() {
}

void CloudLinkTask::onInitialize()  {
    m_pTimeoutTimer = createOneShotTimer(TIMEOUT_SERVICE_MODE_MS);
    m_pWifiStateUpdater = createOneShotTimer(RSSI_UPDATE_PERIOD_MS);
}

void CloudLinkTask::onStart() {
    // nothing to do
}

void CloudLinkTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
    case EventIdentifiers::WIFI_SETTINGS_EVENT: 
        onHandleWifiSettings(WifiSettingsEvent(*pEvent));
        break;

    case EventIdentifiers::DEVICE_SETTINGS_EVENT: 
        onHandleDeviceSettings(DeviceSettingsEvent(*pEvent));
        break;
    
    case EventIdentifiers::SENSOR_DATA_EVENT: 
        onHandleSensorDataEvent(SensorDataEvent(*pEvent));
        break;

    case EventIdentifiers::DEVICE_INFO_EVENT: 
        onHandleDeviceInfo(DeviceStatusEvent(*pEvent));
        break;

    default:
        break;
    }
}

void CloudLinkTask::onHandleTimer(TimerId timerId) {
    if (m_pTimeoutTimer->id == timerId) {
        onHandleTimeout();
    }
    else if (m_pWifiStateUpdater->id == timerId) {
        onHandleWifiStateUpdate();
    }
}

void CloudLinkTask::onHandleWifiSettings(const WifiSettingsEvent& settings) {
    m_wifi.updateWifiSettings(settings);

    if (settings.getMode() == WifiMode::AP) {
        m_wifi.startServiceMode();
        m_pTimeoutTimer->start();
    }
    else if (settings.getMode() == WifiMode::STA) {
        m_wifi.startNormalMode();
        m_pTimeoutTimer->start();
    }
    else {
        ESP_LOGW("CloudLinkTask", "Mode %i not supported", settings.getMode());
    }
}

void CloudLinkTask::onHandleDeviceSettings(const DeviceSettingsEvent& settings) {
    m_wifi.updateDeviceSettings(settings);
}

void CloudLinkTask::onHandleSensorDataEvent(const SensorDataEvent& settings) {
    if (m_lastWifiEvent.getStatus() != WifiStatus::MQTT_CONNECTED) {
        ESP_LOGW("CloudLink", "MQTT not connected, no sensor data sent to MQTT Broker");
        return;
    }

    const HDPRoomStatus roomStatus(
        settings.getRelativeHumidity(),
        settings.getPressure(),
        settings.getTemperature(),
        settings.getGasResistance()
    );

    m_mqttService.publish("devices/$sn/room/info", roomStatus);
}

void CloudLinkTask::onHandleDeviceInfo(const DeviceStatusEvent& status) {
    if (m_lastWifiEvent.getStatus() != WifiStatus::MQTT_CONNECTED) {
        ESP_LOGW("CloudLink", "MQTT not connected, no device info sent to MQTT Broker");
        return;
    }

    const HDPDeviceStatus deviceInfo(
        status.getBatteryStatus().getLevel(),
        status.getWifiStatus().getRssi(),
        status.getUptime()
    );

    m_mqttService.publish("devices/$sn/status", deviceInfo);
}

void CloudLinkTask::onHandleTimeout() {
    m_wifi.onTimeout();    
}

void CloudLinkTask::onHandleWifiStateUpdate() {
    sendWifiStatus(m_lastWifiEvent.getStatus(), m_wifi.getSignalStrength());
}

void CloudLinkTask::sendWifiStatus(const WifiStatus status, int32_t rssi) {
    m_lastWifiEvent.setStatus(status);
    m_lastWifiEvent.setRssi(rssi);

    StatusEvent.send(EventIdentifiers::WIFI_STATUS_EVENT, &m_lastWifiEvent);

    // start new timeout, just ensure wifi status is updated periodicaly
    m_pWifiStateUpdater->start();
}

void CloudLinkTask::onDeviceConfig(const HDPDeviceConfig& msg) {
    ESP_LOGW("CloudLink", "onDeviceConfig. Not implemented yet");
    // TODO
}

void CloudLinkTask::onUpdateInfo(const HDPUpdateInfo& msg) {
    ESP_LOGW("CloudLink", "onUpdateInfo. Not implemented yet");
    // TODO
}
    
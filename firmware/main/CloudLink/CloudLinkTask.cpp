#include "CloudLinkTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiStatusEvent.h"

CloudLinkTask::CloudLinkTask() :
    StatusEvent(),
    m_ctrlHandler(),
    m_mqttService(*this),
    m_wifi(*this, m_ctrlHandler, m_mqttService),
    m_pTimeoutTimer(NULL) {
}

CloudLinkTask::~CloudLinkTask() {
}

void CloudLinkTask::onInitialize()  {
    m_pTimeoutTimer = createOneShotTimer(TIMEOUT_SERVICE_MODE);
}

void CloudLinkTask::onStart() {
}

void CloudLinkTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_SETTINGS_EVENT: {
            WifiSettingsEvent msg(*pEvent);
            onHandleWifiSettings(msg);
        }
        break;

        case EventIdentifiers::DEVICE_SETTINGS_EVENT: {
            DeviceSettingsEvent msg(*pEvent);
            onHandleDeviceSettings(msg);
        }
        break;
        
        case EventIdentifiers::SENSOR_DATA_EVENT: {
            SensorDataEvent msg(*pEvent);
            onHandleSensorDataEvent(msg);
        }
        break;

    default:
        break;
    }
}

void CloudLinkTask::onHandleTimer(TimerId timerId) {
    if (m_pTimeoutTimer->id == timerId) {
        onHandleTimeout();
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

void CloudLinkTask::onHandleSensorDataEvent(const SensorDataEvent& settings) {
    m_mqttService.publish(settings);
}

void CloudLinkTask::onHandleDeviceSettings(const DeviceSettingsEvent& settings) {
    m_wifi.updateDeviceSettings(settings);
}

void CloudLinkTask::onHandleTimeout() {
    m_wifi.onTimeout();    
}

void CloudLinkTask::sendWifiStatus(const WifiStatus status, const int32_t rssi) {
    WifiStatusEvent event;
    event.setStatus(status);
    event.setRssi(rssi);

    StatusEvent.send(EventIdentifiers::WIFI_STATUS_EVENT, &event);
}
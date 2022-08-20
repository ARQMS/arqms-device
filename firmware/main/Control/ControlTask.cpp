#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiSettingsEvent.h"
#include "Events/DeviceSettingsEvent.h"
    
StorageDriverIfc* ControlTask::s_pNvsStorageDriver = NULL;

ControlTask::ControlTask() :
    GuiSettings(),
    WifiSettings(),
    m_coreSm(*this) {
}

ControlTask::~ControlTask() {
}

void ControlTask::onInitialize()  {
    assert(s_pNvsStorageDriver != NULL);
}

void ControlTask::onStart() {
    WifiSettingsEvent wifiSettings;
    s_pNvsStorageDriver->readWifiConfig(&wifiSettings);

    DeviceSettingsEvent deviceSettings;
    s_pNvsStorageDriver->readDeviceConfig(&deviceSettings);

    WifiSettings.send(EventIdentifiers::DEVICE_SETTINGS_EVENT, &deviceSettings);
    WifiSettings.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &wifiSettings);
}

void ControlTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_STATUS_EVENT: {
            WifiStatusEvent msg(*pEvent);
            onHandleWifiStatus(msg);
        }
        break;
        
        case EventIdentifiers::BATTERY_LEVEL_EVENT:{
            BatteryStatusEvent msg(*pEvent);
            onHandleBatteryStatus(msg);
        }
        break;
    default:
        break;
    }
}

void ControlTask::onHandleTimer(const TimerId timerId) {
    // nothing to do
}

void ControlTask::onHandleWifiStatus(const WifiStatusEvent& status) {
    if (status.getStatus() == WifiStatus::CONNECTED) {
        m_coreSm.onConnected();
    }
    else if (status.getStatus() == WifiStatus::STA_TIMEOUT) {
        m_coreSm.onServiceMode();
    }
    else if (status.getStatus() == WifiStatus::CLIENT_DISCONNECTED || status.getStatus() == WifiStatus::CLIENT_TIMEOUT) {
        // TODO perform shutdown, seems IDF does not support shutdown, 
        // so we must connect GPIO 1 (PCU_STATE2) as output and connect to Latch Power or 
        // battery charger
        esp_restart();
    }
}

void ControlTask::onHandleBatteryStatus(const BatteryStatusEvent& status){
    ESP_LOGI("Control Task", "Handle Battery Status Event");
}

void ControlTask::sendWifi(WifiSettingsEvent& wifiEvent) {
    WifiSettings.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &wifiEvent);
}

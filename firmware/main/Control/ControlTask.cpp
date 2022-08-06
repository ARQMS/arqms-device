#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
    
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
    const WifiParameters* wifiConfig = s_pNvsStorageDriver->readWifiConfig();
    if (wifiConfig == NULL || wifiConfig->isWifiCfgValid()) {
        m_coreSm.onServiceMode();
    }
    else {
        m_coreSm.tryConnection(*wifiConfig);
    }
}

void ControlTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_STATUS_EVENT: {
            WifiStatusEvent msg(*pEvent);
            onHandleWifiStatus(msg);
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
    if (status.getWifiStatus() == WifiStatus::CONNECTED) {
        m_coreSm.onConnected();
    }
    else if (status.getWifiStatus() == WifiStatus::CLIENT_DISCONNECTED || status.getWifiStatus() == WifiStatus::CLIENT_TIMEOUT) {
        // TODO perform shutdown, seems IDF does not support shutdown, 
        // so we must connect GPIO 1 (PCU_STATE2) as output and connect to Latch Power or 
        // battery charger
        esp_restart();
    }
}

void ControlTask::sendWifi(WifiSettingsEvent& wifiEvent) {
    WifiSettings.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &wifiEvent);
}

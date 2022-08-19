#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiSettingsEvent.h"
#include "Events/DeviceSettingsEvent.h"
    
StorageDriverIfc* ControlTask::s_pNvsStorageDriver = NULL;

ControlTask::ControlTask() :
    GuiSettings(),
    WifiSettings(),
    m_coreSm() {
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

    if (wifiSettings.getMode() == WifiMode::AP) {
        m_coreSm.onServiceMode();
    } 
    else if (wifiSettings.getMode() == WifiMode::STA) {
        m_coreSm.onRunning();
    }

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
    default:
        break;
    }
}

void ControlTask::onHandleTimer(const TimerId timerId) {
    // nothing to do
}

void ControlTask::onHandleWifiStatus(const WifiStatusEvent& status) {
    if (m_coreSm.isCurrentState(CoreSM::State::Service)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::CLIENT_DISCONNECTED) {
            // TODO perform shutdown, seems IDF does not support shutdown, 
            // so we must connect GPIO 1 (PCU_STATE2) as output and connect to Latch Power or 
            // battery charger
            esp_restart();
        }
    }
    else if (m_coreSm.isCurrentState(CoreSM::State::Running)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::DISCONNECTED) {
            m_coreSm.onServiceMode();

            WifiSettingsEvent event;
            event.setMode(WifiMode::AP);
            WifiSettings.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &event);
        }
    }
}

#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiSettingsEvent.h"

ControlTask::ControlTask() :
    GuiSettings(),
    WifiSettings() {
}

ControlTask::~ControlTask() {
}

void ControlTask::onInitialize()  {
}

void ControlTask::onStart() {
    // TODO replace with proper main state machine (https://github.com/ARQMS/arqms-device/wiki/Firmware#main-statemachine)
    // var wifiConfig = persistancy.readWifiConfig();
    // if (wifiConfig != null)
    //      CloudLink.send(WIFI_SETTINGS_EVENT, &wifiSettings)
    // else
    //      m_deviceStateMachine.onServiceMode()

    // TODO delete demo code
    WifiSettingsEvent wifiSettings;
    wifiSettings.setMode(WifiMode::AP);
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
    if (status.getWifiStatus() == WifiStatus::CONNECTING) {
        // m_deviceStateMachine.onConnecting();
    }
    else if (status.getWifiStatus() == WifiStatus::CONNECTED) {
        // m_deviceStateMachine.onIdle();
    }
    else if (status.getWifiStatus() == WifiStatus::CLIENT_DISCONNECTED || status.getWifiStatus() == WifiStatus::CLIENT_TIMEOUT) {
        // TODO perform shutdown, seems IDF does not support shutdown, 
        // so we must connect GPIO 1 (PCU_STATE2) as output and connect to Latch Power or 
        // battery charger
        esp_restart();
    }
    
}

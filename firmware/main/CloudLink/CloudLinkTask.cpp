#include "CloudLinkTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiStatusEvent.h"

CloudLinkTask::CloudLinkTask() :
    Control(),
    m_wifiStateMachine(*this) {
}

CloudLinkTask::~CloudLinkTask() {
}

void CloudLinkTask::onInitialize()  {
}

void CloudLinkTask::onStart() {
}

void CloudLinkTask::onExecute(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_SETTINGS_EVENT: {
            WifiSettingsEvent msg(*pEvent);
            onHandleWifiSettings(msg);
        }
    default:
        break;
    }
}

void CloudLinkTask::onHandleWifiSettings(const WifiSettingsEvent& settings) {
    if (settings.getMode() == WifiMode::AP) {
        sendWifiStatus(WifiStatus::CLIENT_SEARCHING);

        m_wifiStateMachine.onServiceMode();

        // TODO start timout 1min, when reached before client is connected -> shutdown
    }
    else if (settings.getMode() == WifiMode::STA) {
        // TODO
    }
    else {
        m_wifiStateMachine.reset();
        
        sendWifiStatus(WifiStatus::DISABLED);
    }
}

void CloudLinkTask::onClientConnected() {
    sendWifiStatus(WifiStatus::CLIENT_CONNECTED);
}

void CloudLinkTask::onClientDisconnected() {
    sendWifiStatus(WifiStatus::CLIENT_DISCONNECTED);
}

void CloudLinkTask::onServiceModeIdle() {

}

void CloudLinkTask::onNormalModeIdle() {

}

void CloudLinkTask::sendWifiStatus(const WifiStatus status, const int32_t rssi) {
    WifiStatusEvent event;
    event.setWifiStatus(status);
    event.setRssi(rssi);

    Control.send(EventIdentifiers::WIFI_STATUS_EVENT, &event);
}
#include "CloudLinkTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiStatusEvent.h"

CloudLinkTask::CloudLinkTask() :
    StatusEvent(),
    m_wifiStateMachine(*this) {
}

CloudLinkTask::~CloudLinkTask() {
}

void CloudLinkTask::onInitialize()  {
}

void CloudLinkTask::onStart() {
}

void CloudLinkTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_SETTINGS_EVENT: {
            WifiSettingsEvent msg(*pEvent);
            onHandleWifiSettings(msg);
        }
    default:
        break;
    }
}

void CloudLinkTask::onHandleTimer(TimerId timerId) {
    if (m_timeoutTimer == timerId) {
        onHandleTimeout();
    }
}

void CloudLinkTask::onHandleWifiSettings(const WifiSettingsEvent& settings) {
    if (settings.getMode() == WifiMode::AP) {
        sendWifiStatus(WifiStatus::CLIENT_SEARCHING);

        m_wifiStateMachine.onStartServiceMode();

        m_timeoutTimer = startOneShotTimer(TIMEOUT_SERVICE_MODE);
    }
    else if (settings.getMode() == WifiMode::STA) {
        // TODO
    }
    else {
        m_wifiStateMachine.reset();
        
        sendWifiStatus(WifiStatus::DISABLED);
    }
}

void CloudLinkTask::onHandleTimeout() {
    if (m_wifiStateMachine.isCurrentState(WifiStateMachine::State::SERVICE_WAITING)) {
        sendWifiStatus(WifiStatus::CLIENT_TIMEOUT);
    }
}

void CloudLinkTask::sendWifiStatus(const WifiStatus status, const int32_t rssi) {
    WifiStatusEvent event;
    event.setWifiStatus(status);
    event.setRssi(rssi);

    StatusEvent.send(EventIdentifiers::WIFI_STATUS_EVENT, &event);
}
#include "CloudLinkTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiStatusEvent.h"

CloudLinkTask::CloudLinkTask() :
    StatusEvent(),
    m_ctrlHandler(*this),
    m_wifiStateMachine(*this, m_ctrlHandler),
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
    if (settings.getMode() == WifiMode::AP) {
        m_wifiStateMachine.onStartServiceMode();

        m_pTimeoutTimer->start();
    }
    else if (settings.getMode() == WifiMode::STA) {
        // TODO
    }
    else {
        m_wifiStateMachine.reset();
    }
}

void CloudLinkTask::onHandleTimeout() {
    if (m_wifiStateMachine.isCurrentState(WifiStateMachine::State::SERVICE_WAITING)) {
        sendWifiStatus(WifiStatus::CLIENT_TIMEOUT);

        m_wifiStateMachine.reset();
    }
}

void CloudLinkTask::sendWifiStatus(const WifiStatus status, const int32_t rssi) {
    WifiStatusEvent event;
    event.setWifiStatus(status);
    event.setRssi(rssi);

    StatusEvent.send(EventIdentifiers::WIFI_STATUS_EVENT, &event);
}
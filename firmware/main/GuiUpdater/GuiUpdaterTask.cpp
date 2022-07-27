#include "GuiUpdaterTask.h"

#include "Drivers/ApplicationHardwareConfig.h"
#include "Events/EventIdentifiers.h"

GuiUpdaterTask::GuiUpdaterTask() :
    m_airIndicator(LED_AIR_GOOD, LED_AIR_MOD, LED_AIR_POOR),
    m_pRefreshTimer(NULL),
    m_currentQuality(0.0f),
    m_animationSpeed(0.1f) {
}

GuiUpdaterTask::~GuiUpdaterTask() {
}

void GuiUpdaterTask::onInitialize()  {
}

void GuiUpdaterTask::onStart() {
    m_pRefreshTimer = startPeriodicTimer(REFRESH_RATE);
}

void GuiUpdaterTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::QUALITY_EVENT: {
            AirQualityEvent event(*pEvent);
            onHandleAirQuality(event);
        }
        break;
        case EventIdentifiers::WIFI_STATUS_EVENT: {
            WifiStatusEvent event(*pEvent);
            onHandleWifiStatus(event);
        }

    default:
        break;
    }
}

void GuiUpdaterTask::onHandleTimer(const TimerId timerId) {
    if (m_pRefreshTimer->id == timerId) {
        onHandleRefresh();
    }
}

void GuiUpdaterTask::onHandleAirQuality(const AirQualityEvent& qualityEvent) {
    m_airIndicator.setQuality(qualityEvent.getQuality());
}

void GuiUpdaterTask::onHandleWifiStatus(const WifiStatusEvent& wifiStatus) {
    if (wifiStatus.getWifiStatus() == WifiStatus::CLIENT_SEARCHING) {
        // m_wlanIndicator.startBlink(500, blue); // blink blue with 500ms period

        m_animationSpeed = 0.2f;
    }
    else if (wifiStatus.getWifiStatus() == WifiStatus::CLIENT_CONNECTED) {
        // m_wlanIndicator.startBlink(0, blue); // constant blue
        m_animationSpeed = 0.0f;
        m_currentQuality = .5f;
    } 
    else if (wifiStatus.getWifiStatus() == WifiStatus::CONNECTED) {
        // m_wlanIndicator.startBlink(0, blue); // constant blue
        m_animationSpeed = 0.0f;
    } 
    // else if (wifiStatus.getWifiStatus() == WifiStatus::SENDING) {
    //     // m_wlanIndicator.startBlink(50, yellow); // blink yellow
    // } 
    else {
        m_animationSpeed = 0.0f;
        m_currentQuality = 1.0f;
    }
}

// TODO remove demo code
void GuiUpdaterTask::onHandleRefresh() {
    // animation
    if (m_animationSpeed > 0.0f) {
        m_currentQuality += m_animationSpeed;
        if (m_currentQuality >= 1.0f) {
            m_currentQuality = 0.0f; // reset
        }
    }

    m_airIndicator.setQuality(m_currentQuality);
}

#include "GuiUpdaterTask.h"

#include <HumiDevice.Platform/Math.h>

#include "Drivers/ApplicationHardwareConfig.h"
#include "Events/EventIdentifiers.h"

GuiUpdaterTask::GuiUpdaterTask() :
    m_airIndicator(LED_AIR_GOOD, LED_AIR_MOD, LED_AIR_POOR),
    m_wlanIndicator(0, LED_WLAN),
    m_pRefreshTimer(NULL) {
}

GuiUpdaterTask::~GuiUpdaterTask() {
}

void GuiUpdaterTask::onInitialize()  {
    m_pRefreshTimer = createPeriodicTimer(REFRESH_RATE);
    m_wlanIndicator.initialize();
}

void GuiUpdaterTask::onStart() {
    m_pRefreshTimer->start();
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
        break;

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
        m_wlanIndicator.setColor({0, 0, 255}, 500);
    }
    else if (wifiStatus.getWifiStatus() == WifiStatus::CLIENT_CONNECTED) {
        m_wlanIndicator.setColor({0, 0, 255});
    } 
    else if (wifiStatus.getWifiStatus() == WifiStatus::CONNECTED) {
        m_wlanIndicator.setColor({0, 0, 255});
    } 
    else if (wifiStatus.getWifiStatus() == WifiStatus::UNKNOWN_ERROR) {
        m_wlanIndicator.setColor({255, 0, 0}, 100);
    } 
    // else if (wifiStatus.getWifiStatus() == WifiStatus::SENDING) {
    //     // m_wlanIndicator.startBlink(50, yellow); // blink yellow
    // } 
    else {
    }
}

void GuiUpdaterTask::onHandleRefresh() {
    m_wlanIndicator.refresh();
}


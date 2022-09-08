#include "GuiUpdaterTask.h"

#include <HumiDevice.Platform/Math.h>

#include "Drivers/ApplicationHardwareConfig.h"
#include "Events/EventIdentifiers.h"

GuiUpdaterTask::GuiUpdaterTask() :
    m_airIndicator(LED_AIR_GOOD, LED_AIR_MOD, LED_AIR_POOR),
    m_ctrlIndicator(0, LED_WLAN),
    m_pRefreshTimer(NULL) {
}

GuiUpdaterTask::~GuiUpdaterTask() {
}

void GuiUpdaterTask::onInitialize()  {
    m_pRefreshTimer = createPeriodicTimer(REFRESH_RATE);
    m_ctrlIndicator.initialize();
}

void GuiUpdaterTask::onStart() {
    m_pRefreshTimer->start();
}

void GuiUpdaterTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::QUALITY_EVENT: 
            onHandleAirQuality(AirQualityEvent(*pEvent));
            break;

        case EventIdentifiers::WIFI_STATUS_EVENT: 
            onHandleWifiStatus(WifiStatusEvent(*pEvent));
            break;

        case EventIdentifiers::SENSOR_DATA_EVENT: 
            onHandleSensorData(SensorDataEvent(*pEvent));
            break;
        
        case EventIdentifiers::SENSOR_STATUS: 
            onHandleSensorStatus(SensorStatusEvent(*pEvent));
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

void GuiUpdaterTask::onHandleSensorData(const SensorDataEvent& data) {
    // nothing to do
}

void GuiUpdaterTask::onHandleWifiStatus(const WifiStatusEvent& wifiStatus) {
    if (wifiStatus.getStatus() == WifiStatus::CLIENT_SEARCHING) {
        m_ctrlIndicator.setColor({0, 0, 255}, 500);
    }
    if (wifiStatus.getStatus() == WifiStatus::CONNECTING) {
        m_ctrlIndicator.setColor({0, 0, 255}, 100);
    }
    else if (wifiStatus.getStatus() == WifiStatus::CLIENT_CONNECTED) {
        m_ctrlIndicator.setColor({0, 0, 255});
    } 
    else if (wifiStatus.getStatus() == WifiStatus::CONNECTED) {
        m_ctrlIndicator.setColor({0, 0, 50}, 100);
    } 
    else if (wifiStatus.getStatus() == WifiStatus::MQTT_CONNECTED) {
        m_ctrlIndicator.setColor({0, 0, 255});
    } 
    else if (wifiStatus.getStatus() == WifiStatus::MQTT_DISCONNECTED) {
        m_ctrlIndicator.setColor({0, 0, 50}, 100);
    } 
    else if (wifiStatus.getStatus() == WifiStatus::UNKNOWN_ERROR) {
        m_ctrlIndicator.setColor({255, 0, 0}, 100);
    }
}

void GuiUpdaterTask::onHandleSensorStatus(const SensorStatusEvent& status) {
    if (status.getStatus() == SensorStatus::ACQUIRE) {
        m_ctrlIndicator.setColor({255, 255, 0}, 50);
    } else if (status.getStatus() == SensorStatus::IDLE) {
        m_ctrlIndicator.setColor({0, 0, 255});
    } else if (status.getStatus() == SensorStatus::ERROR){
        m_ctrlIndicator.setColor({255, 0, 0}, 100);
    }
}

void GuiUpdaterTask::onHandleRefresh() {
    m_ctrlIndicator.refresh();
}


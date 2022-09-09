#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiSettingsEvent.h"
#include "Events/SensorStatusEvent.h"
#include "Events/DeviceSettingsEvent.h"
#include "Events/DeviceStatusEvent.h"
    
StorageDriverIfc* ControlTask::s_pNvsStorageDriver = NULL;

ControlTask::ControlTask() :
    Gui(),
    CloudLink(),
    m_coreSm(),
    m_lastWifiStatus(),
    m_lastBatteryStatus(),
    m_wifiReceived(false),
    m_batteryReceived(false) {
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

    CloudLink.send(EventIdentifiers::DEVICE_SETTINGS_EVENT, &deviceSettings);
    CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &wifiSettings);

    // TODO not used anymore, when device goes to sleep mode for deviceSettings.getInterval() ms.
    // after wake up, run all jobs again
    m_pDelayTimer = createOneShotTimer(deviceSettings.getInterval() * 1000);
}

void ControlTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_STATUS_EVENT: 
            onHandleWifiStatus(WifiStatusEvent(*pEvent));
            break;
        
        case EventIdentifiers::BATTERY_LEVEL_EVENT:
            onHandleBatteryStatus(BatteryStatusEvent(*pEvent));
            break;

        case EventIdentifiers::BTN_CTRL_EVENT: 
            onHandleButton(ButtonEvent(*pEvent));
            break;

        // TODO job done, check if all jobs done configure sleep timer
        case EventIdentifiers::SENSOR_STATUS: {
            SensorStatusEvent status(*pEvent);
            if (status.getStatus() == SensorStatus::IDLE) {
                m_pDelayTimer->start();
            }
        }
        break;

    default:
        break;
    }
}

void ControlTask::onHandleTimer(const TimerId timerId) {
    // TODO remove demo code
    if (m_pDelayTimer->id == timerId) {
        startJobs();
    }
}

void ControlTask::onHandleWifiStatus(const WifiStatusEvent& status) {
    m_lastWifiStatus = status;
    m_wifiReceived = true;

    if (m_coreSm.isCurrentState(CoreSM::State::Service)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::CLIENT_DISCONNECTED) {
            // TODO perform shutdown, seems IDF does not support shutdown, 
            // so we must connect GPIO 1 (PCU_STATE2) as output and connect to Latch Power or 
            // battery charger
            esp_restart();
        }
    }
    else if (m_coreSm.isCurrentState(CoreSM::State::BootUp)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::DISCONNECTED) {
            m_coreSm.onServiceMode();

            WifiSettingsEvent event;
            event.setMode(WifiMode::AP);
            CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &event);
        }
        else if (status.getStatus() == WifiStatus::MQTT_CONNECTED) {
            m_coreSm.onRunning();

            startJobs();
        }
    }
    else if (m_coreSm.isCurrentState(CoreSM::State::Running)) {
        if (status.getStatus() == WifiStatus::TIMEOUT
         || status.getStatus() == WifiStatus::DISCONNECTED) {
            m_coreSm.onServiceMode();

            WifiSettingsEvent event;
            event.setMode(WifiMode::AP);
            CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &event);
        }

        sendDeviceStatus();
    }
}

void ControlTask::onHandleBatteryStatus(const BatteryStatusEvent& status) {
    m_lastBatteryStatus = status;
    m_batteryReceived = true;

    sendDeviceStatus();
}

void ControlTask::onHandleButton(const ButtonEvent& button) {
    if (button.getButtonId() == ButtonId::SW_RESET) {
        if (button.getStatus() == ButtonStatus::LONG_PRESS) {
            s_pNvsStorageDriver->reset();
        }
        
        esp_restart();
    }
}

void ControlTask::startJobs() {
    MeasSensor.send(EventIdentifiers::SENSOR_SNAPSHOT);
    sendDeviceStatus();
}

void ControlTask::sendDeviceStatus() {
    // pre condition for sending device status
    if (!m_wifiReceived || !m_batteryReceived || !m_coreSm.isCurrentState(CoreSM::State::Running)) return;

    ESP_LOGI("Control", "DeviceInfo");

    m_wifiReceived = false;
    m_batteryReceived = false;

    uint32_t uptime = pdTICKS_TO_MS(xTaskGetTickCount());
    DeviceStatusEvent deviceInfo(m_lastBatteryStatus, m_lastWifiStatus, uptime);

    CloudLink.send(EventIdentifiers::DEVICE_INFO_EVENT, &deviceInfo);
}
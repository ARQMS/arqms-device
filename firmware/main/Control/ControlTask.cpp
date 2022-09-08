#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiSettingsEvent.h"
#include "Events/SensorStatusEvent.h"
#include "Events/DeviceSettingsEvent.h"
    
StorageDriverIfc* ControlTask::s_pNvsStorageDriver = NULL;

ControlTask::ControlTask() :
    Gui(),
    CloudLink(),
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

    CloudLink.send(EventIdentifiers::DEVICE_SETTINGS_EVENT, &deviceSettings);
    CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &wifiSettings);

    m_pDelayTimer = createOneShotTimer(deviceSettings.getInterval() * 1000);
}

void ControlTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::WIFI_STATUS_EVENT: 
            onHandleWifiStatus(WifiStatusEvent(*pEvent));
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
        MeasSensor.send(EventIdentifiers::SENSOR_SNAPSHOT);
    }
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
            CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &event);
        }
        else if (status.getStatus() == WifiStatus::MQTT_CONNECTED) {
            MeasSensor.send(EventIdentifiers::SENSOR_SNAPSHOT);
        }
    }
}

void ControlTask::onHandleButton(const ButtonEvent& button) {
    ESP_LOGI("Control", "BTN %i, %i", button.getButtonId(), button.getStatus());
}


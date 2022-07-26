#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiSettingsEvent.h"
#include "Events/AirQualityEvent.h"

ControlTask::ControlTask() :
    GuiUpdater(),
    CloudLink(),
    Same(),
    m_currentQuality(0.0f) {
}

ControlTask::~ControlTask() {
}

void ControlTask::onInitialize()  {
}

// TODO replace with proper main state machine (https://github.com/ARQMS/arqms-device/wiki/Firmware#main-statemachine)
void ControlTask::onStart() {
    // TODO 
    // var wifiConfig = persistancy.readWifiConfig();

    // no wifi configuration available, start AP
    WifiSettingsEvent wifiSettings;
    wifiSettings.setMode(WifiMode::AP);
    CloudLink.send(EventIdentifiers::WIFI_SETTINGS_EVENT, &wifiSettings);
}

void ControlTask::onExecute(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::TEST_EVENT: {
            onHandleTestId();
        }
        break;

        case EventIdentifiers::WIFI_STATUS_EVENT: {
            WifiStatusEvent msg(*pEvent);
            onHandleWifiStatus(msg);
        }
        break;
    default:
        break;
    }
}

// TODO remove test code
void ControlTask::onHandleTestId() {
    m_currentQuality += .1f; // quality is getting better
    if (m_currentQuality >= 1.0f) {
        m_currentQuality = 0.0f; // reset
    }

    AirQualityEvent event;
    event.setQuality(m_currentQuality);
    GuiUpdater.send(EventIdentifiers::QUALITY_EVENT, &event);

    CloudLink.send(EventIdentifiers::TEST_EVENT, &event);
    vTaskDelay(150 / portTICK_PERIOD_MS);
    Same.send(EventIdentifiers::TEST_EVENT); // call same handle again after 250ms
}

void ControlTask::onHandleWifiStatus(const WifiStatusEvent& status) {
    if (status.getWifiStatus() == WifiStatus::CLIENT_SEARCHING) {
        // TODO remove test code
        // Start blinking
        Same.send(EventIdentifiers::TEST_EVENT);    
    }
    else if (status.getWifiStatus() == WifiStatus::CLIENT_CONNECTED) {
        // TODO
    }
    else if (status.getWifiStatus() == WifiStatus::CLIENT_DISCONNECTED) {
        // TODO perform shutdown, seems IDF does not support shutdown, 
        // so we must connect GPIO 1 (PCU_STATE2) as output and connect to Latch Power or 
        // battery charger
        esp_restart();
    }
    
}

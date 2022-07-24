#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/AirQualityEvent.h"
#include "Events/WifiSettingsEvent.h"

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

void ControlTask::onStart() {
    // TODO remove test code
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
    // TODO remove test code
    // Start blinking
    Same.send(EventIdentifiers::TEST_EVENT);    
}

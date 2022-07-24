#include "CloudLinkTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/WifiStatusEvent.h"

CloudLinkTask::CloudLinkTask() :
    Control() {
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
        WifiStatusEvent status;
        status.setWifiStatus(WifiStatus::CLIENT_SEARCHING);
        Control.send(EventIdentifiers::WIFI_STATUS_EVENT, &status);
    }
}


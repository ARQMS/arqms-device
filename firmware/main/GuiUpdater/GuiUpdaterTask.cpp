#include "GuiUpdaterTask.h"

#include "Drivers/ApplicationHardwareConfig.h"
#include "Events/EventIdentifiers.h"

GuiUpdaterTask::GuiUpdaterTask() :
    m_airIndicator(LED_AIR_GOOD, LED_AIR_MOD, LED_AIR_POOR) {
}

GuiUpdaterTask::~GuiUpdaterTask() {
}

void GuiUpdaterTask::onInitialize()  {
    m_airIndicator.setQuality(AirQuality::UNKNOWN);
}

void GuiUpdaterTask::onStart() {
    // nothing to do
}

void GuiUpdaterTask::onExecute(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::QUALITY_EVENT: {
            AirQualityEvent event(*pEvent);
            onHandleAirQuality(event);
        }
        break;

    default:
        break;
    }
}

void GuiUpdaterTask::onHandleAirQuality(const AirQualityEvent& qualityEvent) {
    m_airIndicator.setQuality(qualityEvent.getQuality());
}
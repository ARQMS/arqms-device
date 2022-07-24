#include "ControlTask.h"

#include "Events/EventIdentifiers.h"

ControlTask::ControlTask() :
    GuiUpdater(),
    Same(),
    m_currentQuality(AirQuality::UNKNOWN) {
}

ControlTask::~ControlTask() {
}

void ControlTask::onInitialize()  {
    
}

void ControlTask::onStart() {
    Same.send(EventIdentifiers::TEST_EVENT);
}

void ControlTask::onExecute(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::TEST_EVENT: 
            onHandleTestId();
        break;

    default:
        break;
    }
}

void ControlTask::onHandleTestId() {
    m_currentQuality = static_cast<AirQuality>((m_currentQuality + 1) % 4);
    AirQualityEvent event;
    event.setQuality(m_currentQuality);
    GuiUpdater.send(EventIdentifiers::QUALITY_EVENT, &event);

    // TODO remove test code
    vTaskDelay(250 / portTICK_PERIOD_MS);
    Same.send(EventIdentifiers::TEST_EVENT); // call same handle again after 250ms
}
#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
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
    m_currentQuality += .1f; // quality is getting better
    if (m_currentQuality >= 1.0f) {
        m_currentQuality = 0.0f; // reset
    }

    AirQualityEvent event;
    event.setQuality(m_currentQuality);
    GuiUpdater.send(EventIdentifiers::QUALITY_EVENT, &event);

    CloudLink.send(EventIdentifiers::TEST_EVENT, &event);

    // TODO remove test code
    vTaskDelay(100 / portTICK_PERIOD_MS);
    Same.send(EventIdentifiers::TEST_EVENT); // call same handle again after 250ms
}
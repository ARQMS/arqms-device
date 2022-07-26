#include "ControlTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/AirQualityEvent.h"

ControlTask::ControlTask() :
    GuiUpdater(),
    m_currentQuality(0.0f) {
}

ControlTask::~ControlTask() {
}

void ControlTask::onInitialize()  {
    
}

void ControlTask::onStart() {
    m_testTimer = startPeriodicTimer(100);
}

void ControlTask::onExecute(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
        case EventIdentifiers::TIMER_EVENT: {
            TimerEvent event(*pEvent);
            onHandleTestId(event);
        }
        break;

    default:
        break;
    }
}

void ControlTask::onHandleTestId(const TimerEvent& timer) {
    ESP_LOGI("HumiDevice", "EventId %i <--> %i", m_testTimer, timer.getId());
    
    if (timer.getId() != m_testTimer) return;

    m_currentQuality += .1f; // quality is getting better
    if (m_currentQuality >= 1.0f) {
        m_currentQuality = 0.0f; // reset
    }

    AirQualityEvent event;
    event.setQuality(m_currentQuality);
    GuiUpdater.send(EventIdentifiers::QUALITY_EVENT, &event);
}
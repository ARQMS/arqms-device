#include "CloudLinkTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/AirQualityEvent.h"

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
    default:
        break;
    }
}

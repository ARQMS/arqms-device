#include "ControlTask.h"

ControlTask::ControlTask() :
    MeasSensor() {
}

ControlTask::~ControlTask() {
}

void ControlTask::onInitialize()  {
    
}

void ControlTask::onStart() {

}

void ControlTask::onExecute(EventId eventId, EventIfc* pEvent) {
    switch (eventId) {
    default:
        // TODO remove test code
        MeasSensor.send(eventId, pEvent);
        break;
    }
}
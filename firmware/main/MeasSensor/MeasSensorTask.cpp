#include "MeasSensorTask.h"

#include "Events/EventIdentifiers.h"
    
MeasSensorTask::MeasSensorTask() {
}

MeasSensorTask::~MeasSensorTask() {
}

void MeasSensorTask::onInitialize() {
    // TODO initialize BME680 Driver
}

void MeasSensorTask::onStart() {
}

void MeasSensorTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
    case EventIdentifiers::SENSOR_SNAPSHOT:
        onHandleSnapshot();
        break;
        
    default:
        break;
    }
}

void MeasSensorTask::onHandleTimer(const TimerId timerId) {
    // nothing to do
}

void MeasSensorTask::onHandleSnapshot() {
    // TODO request data from BME680

    // TODO send data to Measurement
}
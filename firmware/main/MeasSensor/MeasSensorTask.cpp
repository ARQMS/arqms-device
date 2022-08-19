#include "MeasSensorTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/SensorDataEvent.h"
    
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
    SensorDataEvent event;
    event.setCo2(0.04f);
    event.setPressure(1024.0f);
    event.setRelativeHumidity(0.25f);
    event.setTemperature(24.4f);
    event.setVoc(98.4f);

    for (int i = 0; i < 5; i++) {
        Measurement.send(EventIdentifiers::SENSOR_DATA_EVENT, &event);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
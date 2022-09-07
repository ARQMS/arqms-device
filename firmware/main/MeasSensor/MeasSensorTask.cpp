#include "MeasSensorTask.h"

#include "Events/EventIdentifiers.h"
    
MeasSensorTask::MeasSensorTask() : 
    m_bosch680Sensor(),
    m_pWaitDataTimer(NULL),
    m_burstIdx(0U) {
}

MeasSensorTask::~MeasSensorTask() {
}

void MeasSensorTask::onInitialize() {
    m_bosch680Sensor.initialize();

    // m_pWaitDataTimer = createOneShotTimer(m_bosch680Sensor.getWaitForNewDataMs());
    m_pWaitDataTimer = createOneShotTimer(5000);
}

void MeasSensorTask::onStart() {
    if (!m_bosch680Sensor.selfCheck()) {
        // TODO send sensor error to ControlTask! TBD what happen when ControlTask receives sensor error
        //  - Do not read sensor values
        //  - Notify UI
        //  - Notify CloudLink
    }
}

void MeasSensorTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
    case EventIdentifiers::SENSOR_SNAPSHOT:
        onHandleSnapshot();
        break;
    }
}

void MeasSensorTask::onHandleTimer(const TimerId timerId) {
    if (timerId == m_pWaitDataTimer->id) {
        onHandleDataAvailable();
    }
}

void MeasSensorTask::onHandleSnapshot() {
    m_burstIdx = 0;
    startSnapshot();
}

void MeasSensorTask::onHandleDataAvailable() {
    SensorDataEvent event;
    m_bosch680Sensor.getData(event);

    Measurement.send(EventIdentifiers::SENSOR_DATA_EVENT, &event);

    // start again
    if (m_burstIdx < READ_SENSOR_BURST_NO) {
        startSnapshot();
    }
}

void MeasSensorTask::startSnapshot() {
    m_burstIdx++;

    ESP_LOGI("MeasSensor", "Start snapshot (%i/%i)", m_burstIdx, READ_SENSOR_BURST_NO);

    m_bosch680Sensor.startSnapshot();
    m_pWaitDataTimer->start();
}

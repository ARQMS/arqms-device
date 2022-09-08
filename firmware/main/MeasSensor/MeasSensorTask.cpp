#include "MeasSensorTask.h"

#include "Events/EventIdentifiers.h"
    
MeasSensorTask::MeasSensorTask() : 
    m_bosch680Sensor(),
    m_pWaitDataTimer(NULL),
    m_burstIdx(0U),
    m_initialized(false) {
}

MeasSensorTask::~MeasSensorTask() {
}

void MeasSensorTask::onInitialize() {
    m_bosch680Sensor.initialize();

    m_pWaitDataTimer = createOneShotTimer(2500);
}

void MeasSensorTask::onStart() {
    m_initialized = m_bosch680Sensor.selfCheck();
    if (!m_initialized) {
        ESP_LOGE("MeasSensor", "Self-Check for BME680 sensor failed!");
        sendStatus(SensorStatus::ERROR);
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
    // Sensor is already acquiring data, so do not start again
    if (m_burstIdx != 0) return;

    startSnapshot();
}

void MeasSensorTask::onHandleDataAvailable() {
    SensorDataEvent event;
    m_bosch680Sensor.getData(event);

    Measurement.send(EventIdentifiers::SENSOR_DATA_EVENT, &event);

    // start over READ_SENSOR_BURST_NO times
    if (m_burstIdx < READ_SENSOR_BURST_NO) {
        startSnapshot();
    }
    else {
        sendStatus(SensorStatus::IDLE);
        m_burstIdx = 0;
    }
}

void MeasSensorTask::startSnapshot() {
    if (!m_initialized) {
        ESP_LOGW("MeasSensor", "Sensor is not initialized, no data acquired!");
        return;
    }

    m_burstIdx++;
    sendStatus(SensorStatus::ACQUIRE);

    ESP_LOGI("MeasSensor", "Start snapshot (%i/%i)", m_burstIdx, READ_SENSOR_BURST_NO);

    m_bosch680Sensor.startSnapshot();
    m_pWaitDataTimer->start();
}

void MeasSensorTask::sendStatus(const SensorStatus status) {
    SensorStatusEvent event(status);

    Status.send(EventIdentifiers::SENSOR_STATUS, &event);
}

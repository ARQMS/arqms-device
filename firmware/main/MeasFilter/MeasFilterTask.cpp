#include "MeasFilterTask.h"

#include "Events/EventIdentifiers.h"
    
MeasFilterTask::MeasFilterTask() :
    m_tempFilter(),
    m_pressureFilter(),
    m_gasResistanceFilter(),
    m_humidtyFilter() {
}

MeasFilterTask::~MeasFilterTask() {
}

void MeasFilterTask::onInitialize() {
    // nothing to do
}

void MeasFilterTask::onStart() {
    // nothing to do
}

void MeasFilterTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    switch (eventId) {
    case EventIdentifiers::SENSOR_DATA_EVENT: {
        SensorDataEvent event(*pEvent);
        onHandleSensorData(event);
    }
    break;
    }       
}

void MeasFilterTask::onHandleTimer(const TimerId timerId) {
    // nothing to do
}

void MeasFilterTask::onHandleSensorData(SensorDataEvent& data) {
    m_tempFilter.update(data.getTemperature());
    m_pressureFilter.update(data.getPressure());
    m_gasResistanceFilter.update(data.getGasResistance());
    m_humidtyFilter.update(data.getRelativeHumidity());

    if (m_tempFilter.getCount() >= READ_SENSOR_BURST_NO) {
        publish();

        m_pressureFilter.reset();
        m_humidtyFilter.reset();
        m_tempFilter.reset();
        m_gasResistanceFilter.reset();
    }
}

void MeasFilterTask::publish() {
    SensorDataEvent data(
        m_pressureFilter.get(),
        m_humidtyFilter.get(), 
        m_tempFilter.get(),
        m_gasResistanceFilter.get()
    );

    ESP_LOGI("MeasFilter", "Filtered data Temp:%.2f [°C], Humidity:%.2f [%%], Pressure:%.2f [Pa], GasResistance:%.1f [Ohm]", data.getTemperature(), data.getRelativeHumidity(), data.getPressure(), data.getGasResistance());

    Measurement.send(EventIdentifiers::SENSOR_DATA_EVENT, &data);
}

#include "MeasFilterTask.h"

#include "Events/EventIdentifiers.h"
#include "Events/AirQualityEvent.h"

#include <cmath>
    
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

    process();
}

void MeasFilterTask::process() {
    if (m_tempFilter.getCount() < READ_SENSOR_BURST_NO) return;

    // data processing
    SensorDataEvent data(
        m_pressureFilter.get(),
        m_humidtyFilter.get(), 
        m_tempFilter.get(),
        m_gasResistanceFilter.get()
    );

    m_pressureFilter.reset();
    m_humidtyFilter.reset();
    m_tempFilter.reset();
    m_gasResistanceFilter.reset();
    
    ESP_LOGI("MeasFilter", "Filtered data Temp:%.2f [°C], Humidity:%.2f [%%], Pressure:%.2f [Pa], GasResistance:%.1f [Ohm]", data.getTemperature(), data.getRelativeHumidity(), data.getPressure(), data.getGasResistance());

    Measurement.send(EventIdentifiers::SENSOR_DATA_EVENT, &data);

    // quality calculation
    const float32_t index = calcAirIndex(data.getRelativeHumidity(), data.getTemperature());
    AirQualityEvent quality;
    quality.setQuality(index);

    Measurement.send(EventIdentifiers::QUALITY_EVENT, &quality);
}

float32_t MeasFilterTask::calcAirIndex(const float32_t humidity, const float32_t temperature) const {
    // see matlab script for more details about constants
    const static float32_t WEIGHT_HUMIDITY = 0.002f;
    const static float32_t WEIGHT_TEMP = 0.05f;

    // air quality index 0..1 is calcualted based on custom formula without any science background
    // see https://github.com/ARQMS/arqms-device/wiki/Air-Quality-Index for more details
    const static float32_t TARGET_HUMIDITY = 45.f;
    const static float32_t TARGET_TEMP = 23.f;

    const float32_t errorTempSq = (TARGET_TEMP - temperature)*(TARGET_TEMP - temperature);
    const float32_t errorHumiditySq = (TARGET_HUMIDITY - humidity)*(TARGET_HUMIDITY - humidity);
    const float32_t error = WEIGHT_TEMP*errorTempSq + WEIGHT_HUMIDITY*errorHumiditySq;
    return exp2f(-error);
}

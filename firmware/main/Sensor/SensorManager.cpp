#include "SensorManager.h"

SensorManager::SensorManager(EventLoopIfc& eventLoop) :
    eventLoop(eventLoop) {
}

void SensorManager::startDataAcquisition() {
    // TODO
    // BME680Driver::SensorData data;
    // bme680Driver.acquisition(&data);

    // Publisher::SensorData sensorData;
    // sensorData.pressure = data.pressure;
    // sensorData.temperature = data.temperature;
    // sensorData.voc = data.voc;
    // sensorData.co2 = data.co2;
    // sensorData.humidity = data.humidity;

    // eventLoop.sensorData(sensorData);
}
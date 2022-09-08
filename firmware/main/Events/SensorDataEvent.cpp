#include "SensorDataEvent.h"

#include <HumiDevice.Platform/Math.h>

SensorDataEvent::SensorDataEvent(void) :
    m_pressure(0.0F),
    m_relativeHumidity(0.0F),
    m_temperature(0.0F),
    m_gasResistance(0.0F) {
    // Nothing to do.
}

SensorDataEvent::SensorDataEvent(const float32_t pressure, const float32_t relativeHumidity, const float32_t temperature, const float32_t gasResistance) :
    m_pressure(pressure),
    m_relativeHumidity(relativeHumidity),
    m_temperature(temperature),
    m_gasResistance(gasResistance) {
    // Nothing to do.
}

SensorDataEvent::SensorDataEvent(const SensorDataEvent& other) :
    m_pressure(other.m_pressure),
    m_relativeHumidity(other.m_relativeHumidity),
    m_temperature(other.m_temperature),
    m_gasResistance(other.m_gasResistance) {
    // Nothing to do.
}

SensorDataEvent::SensorDataEvent(Deserializer& deserializer) :
    m_pressure(0.0F),
    m_relativeHumidity(0.0F),
    m_temperature(0.0F),
    m_gasResistance(0.0F) {
    SensorDataEvent::deserialize(deserializer);
}

SensorDataEvent::~SensorDataEvent(void) {
    // Nothing to do.
}

SensorDataEvent& SensorDataEvent::operator=(const SensorDataEvent& other) {
    if (&other == this) {
        return *this;
    }
    m_pressure = other.m_pressure;
    m_relativeHumidity = other.m_relativeHumidity;
    m_temperature = other.m_temperature;
    m_gasResistance = other.m_gasResistance;
    return *this;
}

bool SensorDataEvent::operator==(const SensorDataEvent& right) const {
    return Math::areEqualFloats(this->m_pressure, right.m_pressure) &&
        Math::areEqualFloats(this->m_relativeHumidity, right.m_relativeHumidity) &&
        Math::areEqualFloats(this->m_temperature, right.m_temperature) &&
        Math::areEqualFloats(this->m_gasResistance, right.m_gasResistance);
}

bool SensorDataEvent::operator!=(const SensorDataEvent& right) const {
    return !(*this == right);
}

void SensorDataEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_pressure;
    deserializer >> m_relativeHumidity;
    deserializer >> m_temperature;
    deserializer >> m_gasResistance;
}

void SensorDataEvent::serialize(Serializer& serializer) const {
    serializer << m_pressure;
    serializer << m_relativeHumidity;
    serializer << m_temperature;
    serializer << m_gasResistance;
}

void SensorDataEvent::setPressure(const float32_t pressure) {
    m_pressure = pressure;
}

float32_t SensorDataEvent::getPressure(void) const {
    return m_pressure;
}

void SensorDataEvent::setRelativeHumidity(const float32_t relativeHumidity) {
    m_relativeHumidity = relativeHumidity;
}

float32_t SensorDataEvent::getRelativeHumidity(void) const {
    return m_relativeHumidity;
}

void SensorDataEvent::setTemperature(const float32_t temperature) {
    m_temperature = temperature;
}

float32_t SensorDataEvent::getTemperature(void) const {
    return m_temperature;
}

void SensorDataEvent::setGasResistance(const float32_t gasResistance) {
    m_gasResistance = gasResistance;
}

float32_t SensorDataEvent::getGasResistance(void) const {
    return m_gasResistance;
}


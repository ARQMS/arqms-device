#include "SensorDataEvent.h"

#include <HumiDevice.Platform/Math.h>

SensorDataEvent::SensorDataEvent(void) :
    m_pressure(0.0F),
    m_relativeHumidity(0.0F),
    m_temperature(0.0F),
    m_voc(0.0F),
    m_co2(0.0F) {
    // Nothing to do.
}

SensorDataEvent::SensorDataEvent(const float32_t pressure, const float32_t relativeHumidity, const float32_t temperature, const float32_t voc, const float32_t co2) :
    m_pressure(pressure),
    m_relativeHumidity(relativeHumidity),
    m_temperature(temperature),
    m_voc(voc),
    m_co2(co2) {
    // Nothing to do.
}

SensorDataEvent::SensorDataEvent(const SensorDataEvent& other) :
    m_pressure(other.m_pressure),
    m_relativeHumidity(other.m_relativeHumidity),
    m_temperature(other.m_temperature),
    m_voc(other.m_voc),
    m_co2(other.m_co2) {
    // Nothing to do.
}

SensorDataEvent::SensorDataEvent(Deserializer& deserializer) :
    m_pressure(0.0F),
    m_relativeHumidity(0.0F),
    m_temperature(0.0F),
    m_voc(0.0F),
    m_co2(0.0F) {
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
    m_voc = other.m_voc;
    m_co2 = other.m_co2;
    return *this;
}

bool SensorDataEvent::operator==(const SensorDataEvent& right) const {
    return Math::areEqualFloats(this->m_pressure, right.m_pressure) &&
        Math::areEqualFloats(this->m_relativeHumidity, right.m_relativeHumidity) &&
        Math::areEqualFloats(this->m_temperature, right.m_temperature) &&
        Math::areEqualFloats(this->m_voc, right.m_voc) &&
        Math::areEqualFloats(this->m_co2, right.m_co2);
}

bool SensorDataEvent::operator!=(const SensorDataEvent& right) const {
    return !(*this == right);
}

void SensorDataEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_pressure;
    deserializer >> m_relativeHumidity;
    deserializer >> m_temperature;
    deserializer >> m_voc;
    deserializer >> m_co2;
}

void SensorDataEvent::serialize(Serializer& serializer) const {
    serializer << m_pressure;
    serializer << m_relativeHumidity;
    serializer << m_temperature;
    serializer << m_voc;
    serializer << m_co2;
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

void SensorDataEvent::setVoc(const float32_t voc) {
    m_voc = voc;
}

float32_t SensorDataEvent::getVoc(void) const {
    return m_voc;
}

void SensorDataEvent::setCo2(const float32_t co2) {
    m_co2 = co2;
}

float32_t SensorDataEvent::getCo2(void) const {
    return m_co2;
}

#include "SensorStatusEvent.h"

SensorStatusEvent::SensorStatusEvent(void) :
    m_status(SensorStatus::IDLE) {
    // Nothing to do.
}

SensorStatusEvent::SensorStatusEvent(const SensorStatus status) :
    m_status(status) {
    // Nothing to do.
}

SensorStatusEvent::SensorStatusEvent(const SensorStatusEvent& other) :
    m_status(other.m_status) {
    // Nothing to do.
}

SensorStatusEvent::SensorStatusEvent(Deserializer& deserializer) :
    m_status(SensorStatus::IDLE) {
    SensorStatusEvent::deserialize(deserializer);
}

SensorStatusEvent::~SensorStatusEvent(void) {
    // Nothing to do.
}

SensorStatusEvent& SensorStatusEvent::operator=(const SensorStatusEvent& other) {
    if (&other == this) {
        return *this;
    }
    m_status = other.m_status;
    return *this;
}

bool SensorStatusEvent::operator==(const SensorStatusEvent& right) const {
    return (this->m_status == right.m_status);
}

bool SensorStatusEvent::operator!=(const SensorStatusEvent& right) const {
    return !(*this == right);
}

void SensorStatusEvent::deserialize(Deserializer& deserializer) {
    uint8_t status;
    deserializer >> status;
    m_status = static_cast<SensorStatus>(status);
}

void SensorStatusEvent::serialize(Serializer& serializer) const {
    serializer << static_cast<uint8_t>(m_status);
}

void SensorStatusEvent::setStatus(const SensorStatus status) {
    m_status = status;
}

SensorStatus SensorStatusEvent::getStatus(void) const {
    return m_status;
}

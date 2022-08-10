#include "AirQualityEvent.h"

#include <HumiDevice.Platform/Math.h>

AirQualityEvent::AirQualityEvent(void) :
    m_quality(0.0F) {
    // Nothing to do.
}

AirQualityEvent::AirQualityEvent(const float32_t field) :
    m_quality(field) {
    // Nothing to do.
}

AirQualityEvent::AirQualityEvent(const AirQualityEvent& other) :
    m_quality(other.m_quality) {
    // Nothing to do.
}

AirQualityEvent::AirQualityEvent(Deserializer& deserializer) :
    m_quality(0.0F) {
    AirQualityEvent::deserialize(deserializer);
}

AirQualityEvent::~AirQualityEvent(void) {
    // Nothing to do.
}

AirQualityEvent& AirQualityEvent::operator=(const AirQualityEvent& other) {
    if (&other == this) {
        return *this;
    }
    m_quality = other.m_quality;
    return *this;
}

bool AirQualityEvent::operator==(const AirQualityEvent& right) const {
    return Math::areEqualFloats(this->m_quality, right.m_quality);
}

bool AirQualityEvent::operator!=(const AirQualityEvent& right) const {
    return !(*this == right);
}

void AirQualityEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_quality;
}

void AirQualityEvent::serialize(Serializer& serializer) const {
    serializer << m_quality;
}

void AirQualityEvent::setQuality(const float32_t field) {
    m_quality = Math::truncateToRange(field, 0.f, 1.f);
}

float32_t AirQualityEvent::getQuality(void) const {
    return m_quality;
}

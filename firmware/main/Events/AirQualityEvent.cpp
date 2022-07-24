#include "AirQualityEvent.h"

#include <HumiDevice.Platform/Math.h>

AirQualityEvent::AirQualityEvent() :
    m_quality(0.0f) {
}

AirQualityEvent::AirQualityEvent(Deserializer& deserializer) {
    deserialize(deserializer);
}

AirQualityEvent::~AirQualityEvent() {
}

void AirQualityEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_quality;
}

void AirQualityEvent::serialize(Serializer& serializer) const {
    serializer << m_quality;
}

void AirQualityEvent::setQuality(const float32_t quality) { 
    m_quality = Math::truncateToRange(quality, 0.0f, 1.0f); 
}
float32_t AirQualityEvent::getQuality() const { 
    return m_quality;
}
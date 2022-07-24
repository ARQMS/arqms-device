#include "AirQualityEvent.h"

AirQualityEvent::AirQualityEvent() :
    m_quality(AirQuality::UNKNOWN) {
}

AirQualityEvent::AirQualityEvent(Deserializer& deserializer) {
    deserialize(deserializer);
}

AirQualityEvent::~AirQualityEvent() {
}

void AirQualityEvent::deserialize(Deserializer& deserializer) {
    uint32_t quality;
    deserializer >> quality;
    m_quality = static_cast<AirQuality>(quality);
}

void AirQualityEvent::serialize(Serializer& serializer) const {
    serializer << static_cast<uint32_t>(m_quality);
}
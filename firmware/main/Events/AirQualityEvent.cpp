#include "AirQualityEvent.h"

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
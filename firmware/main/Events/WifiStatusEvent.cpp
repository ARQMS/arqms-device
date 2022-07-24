#include "WifiStatusEvent.h"

WifiStatusEvent::WifiStatusEvent() :
    m_rssi(0),
    m_wifiStatus(WifiStatus::UNKNOWN_ERROR) {
}

WifiStatusEvent::WifiStatusEvent(Deserializer& deserializer) {
    deserialize(deserializer);
}

WifiStatusEvent::~WifiStatusEvent() {
}

void WifiStatusEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_rssi;
    uint8_t status;
    deserializer >> status;
    m_wifiStatus = static_cast<WifiStatus>(status);
}

void WifiStatusEvent::serialize(Serializer& serializer) const {
    serializer << m_rssi;
    serializer << static_cast<uint8_t>(m_wifiStatus);
}
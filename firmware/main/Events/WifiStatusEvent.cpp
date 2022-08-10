#include "WifiStatusEvent.h"

WifiStatusEvent::WifiStatusEvent(void) :
    m_rssi(0LL),
    m_status(WifiStatus::DISABLED) {
    // Nothing to do.
}

WifiStatusEvent::WifiStatusEvent(const int32_t rssi, const WifiStatus status) :
    m_rssi(rssi),
    m_status(status) {
    // Nothing to do.
}

WifiStatusEvent::WifiStatusEvent(const WifiStatusEvent& other) :
    m_rssi(other.m_rssi),
    m_status(other.m_status) {
    // Nothing to do.
}

WifiStatusEvent::WifiStatusEvent(Deserializer& deserializer) :
    m_rssi(0LL),
    m_status(WifiStatus::DISABLED) {
    WifiStatusEvent::deserialize(deserializer);
}

WifiStatusEvent::~WifiStatusEvent(void) {
    // Nothing to do.
}

WifiStatusEvent& WifiStatusEvent::operator=(const WifiStatusEvent& other) {
    if (&other == this) {
        return *this;
    }
    m_rssi = other.m_rssi;
    m_status = other.m_status;
    return *this;
}

bool WifiStatusEvent::operator==(const WifiStatusEvent& right) const {
    return (this->m_rssi == right.m_rssi) &&
        (this->m_status == right.m_status);
}

bool WifiStatusEvent::operator!=(const WifiStatusEvent& right) const {
    return !(*this == right);
}

void WifiStatusEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_rssi;
    uint8_t status;
    deserializer >> status;
    m_status = static_cast<WifiStatus>(status);
}

void WifiStatusEvent::serialize(Serializer& serializer) const {
    serializer << m_rssi;
    serializer << static_cast<uint8_t>(m_status);
}

void WifiStatusEvent::setRssi(const int32_t rssi) {
    m_rssi = rssi;
}

int32_t WifiStatusEvent::getRssi(void) const {
    return m_rssi;
}

void WifiStatusEvent::setStatus(const WifiStatus status) {
    m_status = status;
}

WifiStatus WifiStatusEvent::getStatus(void) const {
    return m_status;
}

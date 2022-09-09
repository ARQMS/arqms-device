#include "WifiStatusEvent.h"

WifiStatusEvent::WifiStatusEvent(void) :
    m_rssi(0),
    m_lastSentMsg(0U),
    m_status(WifiStatus::DISABLED) {
    // Nothing to do.
}

WifiStatusEvent::WifiStatusEvent(const int32_t rssi, const uint8_t lastSentMsg, const WifiStatus status) :
    m_rssi(rssi),
    m_lastSentMsg(lastSentMsg),
    m_status(status) {
    // Nothing to do.
}

WifiStatusEvent::WifiStatusEvent(const WifiStatusEvent& other) :
    m_rssi(other.m_rssi),
    m_lastSentMsg(other.m_lastSentMsg),
    m_status(other.m_status) {
    // Nothing to do.
}

WifiStatusEvent::WifiStatusEvent(Deserializer& deserializer) :
    m_rssi(0),
    m_lastSentMsg(0U),
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
    m_lastSentMsg = other.m_lastSentMsg;
    m_status = other.m_status;
    return *this;
}

bool WifiStatusEvent::operator==(const WifiStatusEvent& right) const {
    return (this->m_rssi == right.m_rssi) &&
        (this->m_lastSentMsg == right.m_lastSentMsg) &&
        (this->m_status == right.m_status);
}

bool WifiStatusEvent::operator!=(const WifiStatusEvent& right) const {
    return !(*this == right);
}

void WifiStatusEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_rssi;
    deserializer >> m_lastSentMsg;
    uint8_t status;
    deserializer >> status;
    m_status = static_cast<WifiStatus>(status);
}

void WifiStatusEvent::serialize(Serializer& serializer) const {
    serializer << m_rssi;
    serializer << m_lastSentMsg;
    serializer << static_cast<uint8_t>(m_status);
}

void WifiStatusEvent::setRssi(const int32_t rssi) {
    m_rssi = rssi;
}

int32_t WifiStatusEvent::getRssi(void) const {
    return m_rssi;
}

void WifiStatusEvent::setLastSentMsg(const uint8_t sentMsg) {
    m_lastSentMsg = sentMsg;
}

uint8_t WifiStatusEvent::getLastSentMsg(void) const {
    return m_lastSentMsg;
}

void WifiStatusEvent::setStatus(const WifiStatus status) {
    m_status = status;
}

WifiStatus WifiStatusEvent::getStatus(void) const {
    return m_status;
}

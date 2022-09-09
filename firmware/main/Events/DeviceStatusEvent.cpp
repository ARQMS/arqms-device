#include "DeviceStatusEvent.h"

DeviceStatusEvent::DeviceStatusEvent(void):
    m_batteryStatus(),
    m_wifiStatus(),
    m_uptime(0U) {

}

DeviceStatusEvent::DeviceStatusEvent(BatteryStatusEvent& batteryStatus, WifiStatusEvent& wifiStatus, uint32_t uptime):
    m_batteryStatus(batteryStatus),
    m_wifiStatus(wifiStatus),
    m_uptime(uptime) {
}

DeviceStatusEvent::DeviceStatusEvent(const DeviceStatusEvent& other) :
    m_batteryStatus(other.m_batteryStatus),
    m_wifiStatus(other.m_wifiStatus),
    m_uptime(other.m_uptime) {
}

DeviceStatusEvent::DeviceStatusEvent(Deserializer& deserializer){
    DeviceStatusEvent::deserialize(deserializer);
}

DeviceStatusEvent::~DeviceStatusEvent(void){
    // Nothing to do
}

DeviceStatusEvent& DeviceStatusEvent::operator=(const DeviceStatusEvent& other){
    if(&other == this){
        return *this;
    }
    m_batteryStatus = other.m_batteryStatus;
    m_wifiStatus = other.m_wifiStatus;
    m_uptime = other.m_uptime;
    return *this;
}

bool DeviceStatusEvent::operator==(const DeviceStatusEvent& right) const{
    return this->m_batteryStatus == right.m_batteryStatus
        && this->m_wifiStatus == right.m_wifiStatus
        && this->m_uptime == right.m_uptime;
}

bool DeviceStatusEvent::operator!=(const DeviceStatusEvent& right) const{
    return !(*this == right);
}

void DeviceStatusEvent::deserialize(Deserializer& deserializer){
    deserializer >> m_batteryStatus;
    deserializer >> m_wifiStatus;
    deserializer >> m_uptime;
}

void DeviceStatusEvent::serialize(Serializer& serializer) const{
    serializer << m_batteryStatus;
    serializer << m_wifiStatus;
    serializer << m_uptime;
}

void DeviceStatusEvent::setBatteryStatus(const BatteryStatusEvent& status) {
    m_batteryStatus = status;
}

const BatteryStatusEvent& DeviceStatusEvent::getBatteryStatus(void) const {
    return m_batteryStatus;
}

void DeviceStatusEvent::setWifiStatus(const WifiStatusEvent& status) {
    m_wifiStatus = status;
}

const WifiStatusEvent& DeviceStatusEvent::getWifiStatus(void) const {
    return m_wifiStatus;
}

void DeviceStatusEvent::setUptime(const uint32_t uptime) {
    m_uptime = uptime;
}

uint32_t DeviceStatusEvent::getUptime(void) const {
    return m_uptime;
}

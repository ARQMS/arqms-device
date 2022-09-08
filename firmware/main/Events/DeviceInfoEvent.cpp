#include "DeviceInfoEvent.h"

DeviceInfoEvent::DeviceInfoEvent(void):
    m_batteryStatus(),
    m_wifiStatus(),
    m_uptime(0U) {

}

DeviceInfoEvent::DeviceInfoEvent(BatteryStatusEvent& batteryStatus, WifiStatusEvent& wifiStatus, uint32_t uptime):
    m_batteryStatus(batteryStatus),
    m_wifiStatus(wifiStatus),
    m_uptime(uptime) {
}

DeviceInfoEvent::DeviceInfoEvent(const DeviceInfoEvent& other) :
    m_batteryStatus(other.m_batteryStatus),
    m_wifiStatus(other.m_wifiStatus),
    m_uptime(other.m_uptime) {
}

DeviceInfoEvent::DeviceInfoEvent(Deserializer& deserializer){
    DeviceInfoEvent::deserialize(deserializer);
}

DeviceInfoEvent::~DeviceInfoEvent(void){
    // Nothing to do
}

DeviceInfoEvent& DeviceInfoEvent::operator=(const DeviceInfoEvent& other){
    if(&other == this){
        return *this;
    }
    m_batteryStatus = other.m_batteryStatus;
    m_wifiStatus = other.m_wifiStatus;
    m_uptime = other.m_uptime;
    return *this;
}

bool DeviceInfoEvent::operator==(const DeviceInfoEvent& right) const{
    return this->m_batteryStatus == right.m_batteryStatus
        && this->m_wifiStatus == right.m_wifiStatus
        && this->m_uptime == right.m_uptime;
}

bool DeviceInfoEvent::operator!=(const DeviceInfoEvent& right) const{
    return !(*this == right);
}

void DeviceInfoEvent::deserialize(Deserializer& deserializer){
    deserializer >> m_batteryStatus;
    deserializer >> m_wifiStatus;
    deserializer >> m_uptime;
}

void DeviceInfoEvent::serialize(Serializer& serializer) const{
    serializer << m_batteryStatus;
    serializer << m_wifiStatus;
    serializer << m_uptime;
}

void DeviceInfoEvent::setBatteryStatus(const BatteryStatusEvent& status) {
    m_batteryStatus = status;
}

const BatteryStatusEvent& DeviceInfoEvent::getBatteryStatus(void) const {
    return m_batteryStatus;
}

void DeviceInfoEvent::setWifiStatus(const WifiStatusEvent& status) {
    m_wifiStatus = status;
}

const WifiStatusEvent& DeviceInfoEvent::getWifiStatus(void) const {
    return m_wifiStatus;
}

void DeviceInfoEvent::setUptime(const uint32_t uptime) {
    m_uptime = uptime;
}

uint32_t DeviceInfoEvent::getUptime(void) const {
    return m_uptime;
}

#include "DeviceSettingsEvent.h"

DeviceSettingsEvent::DeviceSettingsEvent() {
    memset(&m_Sn[0], 0, MAX_SN_LENGTH * sizeof(char8_t));
    memset(&m_brokerUri[0], 0, MAX_BROKER_URI_LENGTH * sizeof(char8_t));
}

DeviceSettingsEvent::DeviceSettingsEvent(Deserializer& deserializer) {
    deserialize(deserializer);
}

DeviceSettingsEvent::~DeviceSettingsEvent() {
}

void DeviceSettingsEvent::deserialize(Deserializer& deserializer) {
    for (auto i = 0U; i < MAX_SN_LENGTH; i++) {
        deserializer >> m_Sn[i];
    }
    for (auto i = 0U; i < MAX_BROKER_URI_LENGTH; i++) {
        deserializer >> m_brokerUri[i];
    }
}

void DeviceSettingsEvent::serialize(Serializer& serializer) const {
    for (auto i = 0U; i < MAX_SN_LENGTH; i++) {
        serializer << m_Sn[i];
    }
    for (auto i = 0U; i < MAX_BROKER_URI_LENGTH; i++) {
        serializer << m_brokerUri[i];
    }
}

void DeviceSettingsEvent::setSn(const char8_t password[MAX_SN_LENGTH]) {       
    if (password != NULL) {
        mempcpy(&m_Sn[0], &password[0], MAX_SN_LENGTH * sizeof(char8_t));
    } 
}

void DeviceSettingsEvent::getSn(char8_t password[MAX_SN_LENGTH]) const { 
    if (password != NULL) {
        mempcpy(&password[0], &m_Sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    }
}

void DeviceSettingsEvent::setBrokerUri(const char8_t brokerUri[MAX_BROKER_URI_LENGTH]) {
    if (brokerUri != NULL) {
        mempcpy(&m_brokerUri[0], &brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    } 
}

void DeviceSettingsEvent::getBrokerUri(char8_t brokerUri[MAX_BROKER_URI_LENGTH]) const {
    if (brokerUri != NULL) {
        mempcpy(&brokerUri[0], &m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    }
}

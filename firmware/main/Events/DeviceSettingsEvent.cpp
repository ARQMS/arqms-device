#include "DeviceSettingsEvent.h"

DeviceSettingsEvent::DeviceSettingsEvent(void) {
    memset(&m_brokerUri[0], 0, MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memset(&m_sn[0], 0, MAX_SN_LENGTH * sizeof(char8_t));
}

DeviceSettingsEvent::DeviceSettingsEvent(const char8_t brokerUri[MAX_BROKER_URI_LENGTH], const char8_t sn[MAX_SN_LENGTH]) {
    if (brokerUri != NULL) {
        memcpy(&m_brokerUri[0], &brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_brokerUri[0], 0, MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    }
    if (sn != NULL) {
        memcpy(&m_sn[0], &sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_sn[0], 0, MAX_SN_LENGTH * sizeof(char8_t));
    }
}

DeviceSettingsEvent::DeviceSettingsEvent(const DeviceSettingsEvent& other) {
    memcpy(&m_brokerUri[0], &other.m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memcpy(&m_sn[0], &other.m_sn[0], MAX_SN_LENGTH * sizeof(char8_t));
}

DeviceSettingsEvent::DeviceSettingsEvent(Deserializer& deserializer) {
    DeviceSettingsEvent::deserialize(deserializer);
}

DeviceSettingsEvent::~DeviceSettingsEvent(void) {
    // Nothing to do.
}

DeviceSettingsEvent& DeviceSettingsEvent::operator=(const DeviceSettingsEvent& other) {
    if (&other == this) {
        return *this;
    }
    memcpy(&m_brokerUri[0], &other.m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memcpy(&m_sn[0], &other.m_sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    return *this;
}

bool DeviceSettingsEvent::operator==(const DeviceSettingsEvent& right) const {
    return (memcmp(this->m_brokerUri, right.m_brokerUri, MAX_BROKER_URI_LENGTH * sizeof(char8_t)) == 0) &&
        (memcmp(this->m_sn, right.m_sn, MAX_SN_LENGTH * sizeof(char8_t)) == 0);
}

bool DeviceSettingsEvent::operator!=(const DeviceSettingsEvent& right) const {
    return !(*this == right);
}

void DeviceSettingsEvent::deserialize(Deserializer& deserializer) {
    for (uint32_t i = 0U; i < MAX_BROKER_URI_LENGTH; i++) {
        deserializer >> m_brokerUri[i];
    }
    for (uint32_t i = 0U; i < MAX_SN_LENGTH; i++) {
        deserializer >> m_sn[i];
    }
}

void DeviceSettingsEvent::serialize(Serializer& serializer) const {
    for (uint32_t i = 0U; i < MAX_BROKER_URI_LENGTH; i++) {
        serializer << m_brokerUri[i];
    }
    for (uint32_t i = 0U; i < MAX_SN_LENGTH; i++) {
        serializer << m_sn[i];
    }
}

void DeviceSettingsEvent::setBrokerUri(const char8_t brokerUri[MAX_BROKER_URI_LENGTH]) {
    if (brokerUri != NULL) {
        memcpy(&m_brokerUri[0], &brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    }
}

void DeviceSettingsEvent::getBrokerUri(char8_t brokerUri[MAX_BROKER_URI_LENGTH]) const {
    if (brokerUri != NULL) {
        memcpy(&brokerUri[0], &m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    }
}

void DeviceSettingsEvent::setSn(const char8_t sn[MAX_SN_LENGTH]) {
    if (sn != NULL) {
        memcpy(&m_sn[0], &sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    }
}

void DeviceSettingsEvent::getSn(char8_t sn[MAX_SN_LENGTH]) const {
    if (sn != NULL) {
        memcpy(&sn[0], &m_sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    }
}


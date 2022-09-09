#include "DeviceInfoEvent.h"

DeviceInfoEvent::DeviceInfoEvent(void) :
    m_interval(0U) {
    memset(&m_brokerUri[0], 0, MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memset(&m_sn[0], 0, MAX_SN_LENGTH * sizeof(char8_t));
}

DeviceInfoEvent::DeviceInfoEvent(const char8_t brokerUri[MAX_BROKER_URI_LENGTH], const char8_t channel[MAX_CHANNEL_LENGTH], const char8_t sn[MAX_SN_LENGTH], const uint32_t interval) :
    m_interval(interval) {
    if (brokerUri != NULL) {
        memcpy(&m_brokerUri[0], &brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_brokerUri[0], 0, MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    }
    if (channel != NULL) {
        memcpy(&m_channel[0], &channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_channel[0], 0, MAX_CHANNEL_LENGTH * sizeof(char8_t));
    }
    if (sn != NULL) {
        memcpy(&m_sn[0], &sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_sn[0], 0, MAX_SN_LENGTH * sizeof(char8_t));
    }
}

DeviceInfoEvent::DeviceInfoEvent(const DeviceInfoEvent& other) :
    m_interval(other.m_interval) {
    memcpy(&m_brokerUri[0], &other.m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memcpy(&m_channel[0], &other.m_channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    memcpy(&m_sn[0], &other.m_sn[0], MAX_SN_LENGTH * sizeof(char8_t));
}

DeviceInfoEvent::DeviceInfoEvent(Deserializer& deserializer) {
    DeviceInfoEvent::deserialize(deserializer);
}

DeviceInfoEvent::~DeviceInfoEvent(void) {
    // Nothing to do.
}

DeviceInfoEvent& DeviceInfoEvent::operator=(const DeviceInfoEvent& other) {
    if (&other == this) {
        return *this;
    }
    memcpy(&m_brokerUri[0], &other.m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memcpy(&m_channel[0], &other.m_channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    memcpy(&m_sn[0], &other.m_sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    m_interval = other.m_interval;

    return *this;
}

bool DeviceInfoEvent::operator==(const DeviceInfoEvent& right) const {
    return m_interval != right.m_interval &&
        (memcmp(this->m_brokerUri, right.m_brokerUri, MAX_BROKER_URI_LENGTH * sizeof(char8_t)) == 0) &&
        (memcmp(this->m_channel, right.m_channel, MAX_CHANNEL_LENGTH * sizeof(char8_t)) == 0) &&
        (memcmp(this->m_sn, right.m_sn, MAX_SN_LENGTH * sizeof(char8_t)) == 0);
}

bool DeviceInfoEvent::operator!=(const DeviceInfoEvent& right) const {
    return !(*this == right);
}

void DeviceInfoEvent::deserialize(Deserializer& deserializer) {
    for (uint32_t i = 0U; i < MAX_BROKER_URI_LENGTH; i++) {
        deserializer >> m_brokerUri[i];
    }
    for (uint32_t i = 0U; i < MAX_CHANNEL_LENGTH; i++) {
        deserializer >> m_channel[i];
    }
    for (uint32_t i = 0U; i < MAX_SN_LENGTH; i++) {
        deserializer >> m_sn[i];
    }
    deserializer >> m_interval;
}

void DeviceInfoEvent::serialize(Serializer& serializer) const {
    for (uint32_t i = 0U; i < MAX_BROKER_URI_LENGTH; i++) {
        serializer << m_brokerUri[i];
    }  
    for (uint32_t i = 0U; i < MAX_CHANNEL_LENGTH; i++) {
        serializer << m_channel[i];
    }
    for (uint32_t i = 0U; i < MAX_SN_LENGTH; i++) {
        serializer << m_sn[i];
    }
    serializer << m_interval;
}

void DeviceInfoEvent::setBrokerUri(const char8_t brokerUri[MAX_BROKER_URI_LENGTH]) {
    if (brokerUri != NULL) {
        memcpy(&m_brokerUri[0], &brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    }
}

void DeviceInfoEvent::getBrokerUri(char8_t brokerUri[MAX_BROKER_URI_LENGTH]) const {
    if (brokerUri != NULL) {
        memcpy(&brokerUri[0], &m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    }
}

void DeviceInfoEvent::setChannel(const char8_t channel[MAX_CHANNEL_LENGTH]) {
    if (channel != NULL) {
        memcpy(&m_channel[0], &channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    }
}

void DeviceInfoEvent::getChannel(char8_t channel[MAX_CHANNEL_LENGTH]) const {
    if (channel != NULL) {
        memcpy(&channel[0], &m_channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    }
}

void DeviceInfoEvent::setSn(const char8_t sn[MAX_SN_LENGTH]) {
    if (sn != NULL) {
        memcpy(&m_sn[0], &sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    }
}

void DeviceInfoEvent::getSn(char8_t sn[MAX_SN_LENGTH]) const {
    if (sn != NULL) {
        memcpy(&sn[0], &m_sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    }
}

void DeviceInfoEvent::setInterval(const uint32_t interval) {
    m_interval = interval;
}

uint32_t DeviceInfoEvent::getInterval() const {
    return m_interval;
}

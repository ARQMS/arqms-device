#include "DeviceSettingsEvent.h"

DeviceSettingsEvent::DeviceSettingsEvent(void) :
    m_interval(0U) {
    memset(&m_brokerUri[0], 0, MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memset(&m_sn[0], 0, MAX_SN_LENGTH * sizeof(char8_t));
    memset(&m_room[0], 0, MAX_ROOM_LENGTH * sizeof(char8_t));
}

DeviceSettingsEvent::DeviceSettingsEvent(const char8_t brokerUri[MAX_BROKER_URI_LENGTH], const char8_t sn[MAX_SN_LENGTH], const char8_t room[MAX_ROOM_LENGTH], const uint32_t interval) :
    m_interval(interval) {
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
    if (room != NULL) {
        memcpy(&m_room[0], &room[0], MAX_ROOM_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_room[0], 0, MAX_ROOM_LENGTH * sizeof(char8_t));
    }
}

DeviceSettingsEvent::DeviceSettingsEvent(const DeviceSettingsEvent& other) :
    m_interval(other.m_interval) {
    memcpy(&m_brokerUri[0], &other.m_brokerUri[0], MAX_BROKER_URI_LENGTH * sizeof(char8_t));
    memcpy(&m_sn[0], &other.m_sn[0], MAX_SN_LENGTH * sizeof(char8_t));
    memcpy(&m_room[0], &other.m_room[0], MAX_ROOM_LENGTH * sizeof(char8_t));
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
    memcpy(&m_room[0], &other.m_room[0], MAX_ROOM_LENGTH * sizeof(char8_t));
    m_interval = other.m_interval;

    return *this;
}

bool DeviceSettingsEvent::operator==(const DeviceSettingsEvent& right) const {
    return m_interval != right.m_interval &&
        (memcmp(this->m_brokerUri, right.m_brokerUri, MAX_BROKER_URI_LENGTH * sizeof(char8_t)) == 0) &&
        (memcmp(this->m_sn, right.m_sn, MAX_SN_LENGTH * sizeof(char8_t)) == 0) &&
        (memcmp(this->m_room, right.m_room, MAX_ROOM_LENGTH * sizeof(char8_t)) == 0);
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
    for (uint32_t i = 0U; i < MAX_ROOM_LENGTH; i++) {
        deserializer >> m_room[i];
    }
    deserializer >> m_interval;
}

void DeviceSettingsEvent::serialize(Serializer& serializer) const {
    for (uint32_t i = 0U; i < MAX_BROKER_URI_LENGTH; i++) {
        serializer << m_brokerUri[i];
    }
    for (uint32_t i = 0U; i < MAX_SN_LENGTH; i++) {
        serializer << m_sn[i];
    }
    for (uint32_t i = 0U; i < MAX_ROOM_LENGTH; i++) {
        serializer << m_room[i];
    }
    serializer << m_interval;
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

void DeviceSettingsEvent::setRoom(const char8_t room[MAX_ROOM_LENGTH]) {
    if (room != NULL) {
        memcpy(&m_room[0], &room[0], MAX_ROOM_LENGTH * sizeof(char8_t));
    }
}

void DeviceSettingsEvent::getRoom(char8_t room[MAX_ROOM_LENGTH]) const {
    if (room != NULL) {
        memcpy(&room[0], &m_room[0], MAX_ROOM_LENGTH * sizeof(char8_t));
    }
}

void DeviceSettingsEvent::setInterval(const uint32_t interval) {
    m_interval = interval;
}

uint32_t DeviceSettingsEvent::getInterval() const {
    return m_interval;
}

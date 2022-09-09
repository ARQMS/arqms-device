#include "DeviceConfigEvent.h"

DeviceConfigEvent::DeviceConfigEvent(void) :
    m_interval(0U) {
    memset(&m_channel[0], 0, MAX_CHANNEL_LENGTH * sizeof(char8_t));
}

DeviceConfigEvent::DeviceConfigEvent(const char8_t channel[MAX_CHANNEL_LENGTH], const uint32_t interval) :
    m_interval(interval) {
    if (channel != NULL) {
        memcpy(&m_channel[0], &channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_channel[0], 0, MAX_CHANNEL_LENGTH * sizeof(char8_t));
    }
}

DeviceConfigEvent::DeviceConfigEvent(const DeviceConfigEvent& other) :
    m_interval(other.m_interval) {
    memcpy(&m_channel[0], &other.m_channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
}

DeviceConfigEvent::DeviceConfigEvent(Deserializer& deserializer) {
    DeviceConfigEvent::deserialize(deserializer);
}

DeviceConfigEvent::~DeviceConfigEvent(void) {
    // Nothing to do.
}

DeviceConfigEvent& DeviceConfigEvent::operator=(const DeviceConfigEvent& other) {
    if (&other == this) {
        return *this;
    }
    memcpy(&m_channel[0], &other.m_channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    m_interval = other.m_interval;

    return *this;
}

bool DeviceConfigEvent::operator==(const DeviceConfigEvent& right) const {
    return m_interval != right.m_interval &&
        (memcmp(this->m_channel, right.m_channel, MAX_CHANNEL_LENGTH * sizeof(char8_t)) == 0);
}

bool DeviceConfigEvent::operator!=(const DeviceConfigEvent& right) const {
    return !(*this == right);
}

void DeviceConfigEvent::deserialize(Deserializer& deserializer) {
    for (uint32_t i = 0U; i < MAX_CHANNEL_LENGTH; i++) {
        deserializer >> m_channel[i];
    }
    deserializer >> m_interval;
}

void DeviceConfigEvent::serialize(Serializer& serializer) const {
    for (uint32_t i = 0U; i < MAX_CHANNEL_LENGTH; i++) {
        serializer << m_channel[i];
    }
    serializer << m_interval;
}

void DeviceConfigEvent::setChannel(const char8_t channel[MAX_CHANNEL_LENGTH]) {
    if (channel != NULL) {
        memcpy(&m_channel[0], &channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    }
}

void DeviceConfigEvent::getChannel(char8_t channel[MAX_CHANNEL_LENGTH]) const {
    if (channel != NULL) {
        memcpy(&channel[0], &m_channel[0], MAX_CHANNEL_LENGTH * sizeof(char8_t));
    }
}

void DeviceConfigEvent::setInterval(const uint32_t interval) {
    m_interval = interval;
}

uint32_t DeviceConfigEvent::getInterval() const {
    return m_interval;
}

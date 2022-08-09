#include "WifiSettingsEvent.h"


WifiSettingsEvent::WifiSettingsEvent(void) :
    m_mode(WifiMode::NONE) {
    memset(&m_ssid[0], 0, MAX_SSID_LENGTH * sizeof(char8_t));
    memset(&m_password[0], 0, MAX_PWD_LENGTH * sizeof(char8_t));
}

WifiSettingsEvent::WifiSettingsEvent(const WifiMode mode, const char8_t ssid[MAX_SSID_LENGTH], const char8_t password[MAX_PWD_LENGTH]) :
    m_mode(mode) {
    if (ssid != NULL) {
        memcpy(&m_ssid[0], &ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_ssid[0], 0, MAX_SSID_LENGTH * sizeof(char8_t));
    }
    if (password != NULL) {
        memcpy(&m_password[0], &password[0], MAX_PWD_LENGTH * sizeof(char8_t));
    } else {
        memset(&m_password[0], 0, MAX_PWD_LENGTH * sizeof(char8_t));
    }
}

WifiSettingsEvent::WifiSettingsEvent(const WifiSettingsEvent& other) :
    m_mode(other.m_mode) {
    memcpy(&m_ssid[0], &other.m_ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
    memcpy(&m_password[0], &other.m_password[0], MAX_PWD_LENGTH * sizeof(char8_t));
}

WifiSettingsEvent::WifiSettingsEvent(Deserializer& deserializer) :
    m_mode(WifiMode::NONE) {
    WifiSettingsEvent::deserialize(deserializer);
}

WifiSettingsEvent::~WifiSettingsEvent(void) {
    // Nothing to do.
}

WifiSettingsEvent& WifiSettingsEvent::operator=(const WifiSettingsEvent& other) {
    if (&other == this) {
        return *this;
    }
    m_mode = other.m_mode;
    memcpy(&m_ssid[0], &other.m_ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
    memcpy(&m_password[0], &other.m_password[0], MAX_PWD_LENGTH * sizeof(char8_t));
    return *this;
}

bool WifiSettingsEvent::operator==(const WifiSettingsEvent& right) const {
    return (this->m_mode == right.m_mode) &&
        (memcmp(this->m_ssid, right.m_ssid, MAX_SSID_LENGTH * sizeof(char8_t)) == 0) &&
        (memcmp(this->m_password, right.m_password, MAX_PWD_LENGTH * sizeof(char8_t)) == 0);
}

bool WifiSettingsEvent::operator!=(const WifiSettingsEvent& right) const {
    return !(*this == right);
}

void WifiSettingsEvent::deserialize(Deserializer& deserializer) {
    uint8_t mode;
    deserializer >> mode;
    m_mode = static_cast<WifiMode>(mode);
    for (uint32_t i = 0U; i < MAX_SSID_LENGTH; i++) {
        deserializer >> m_ssid[i];
    }
    for (uint32_t i = 0U; i < MAX_PWD_LENGTH; i++) {
        deserializer >> m_password[i];
    }
}

void WifiSettingsEvent::serialize(Serializer& serializer) const {
    serializer << static_cast<uint8_t>(m_mode);
    for (uint32_t i = 0U; i < MAX_SSID_LENGTH; i++) {
        serializer << m_ssid[i];
    }
    for (uint32_t i = 0U; i < MAX_PWD_LENGTH; i++) {
        serializer << m_password[i];
    }
}

void WifiSettingsEvent::setMode(const WifiMode mode) {
    m_mode = mode;
}

WifiMode WifiSettingsEvent::getMode(void) const {
    return m_mode;
}

void WifiSettingsEvent::setSsid(const char8_t ssid[MAX_SSID_LENGTH]) {
    if (ssid != NULL) {
        memcpy(&m_ssid[0], &ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
    }
}

void WifiSettingsEvent::getSsid(char8_t ssid[MAX_SSID_LENGTH]) const {
    if (ssid != NULL) {
        memcpy(&ssid[0], &m_ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
    }
}

void WifiSettingsEvent::setPassword(const char8_t password[MAX_PWD_LENGTH]) {
    if (password != NULL) {
        memcpy(&m_password[0], &password[0], MAX_PWD_LENGTH * sizeof(char8_t));
    }
}

void WifiSettingsEvent::getPassword(char8_t password[MAX_PWD_LENGTH]) const {
    if (password != NULL) {
        memcpy(&password[0], &m_password[0], MAX_PWD_LENGTH * sizeof(char8_t));
    }
}
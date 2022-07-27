#include "WifiSettingsEvent.h"

WifiSettingsEvent::WifiSettingsEvent() :
    m_mode(WifiMode::STA) {
    memset(&m_ssid[0], 0, MAX_SSID_LENGTH * sizeof(char8_t));
    memset(&m_pwd[0], 0, MAX_PWD_LENGTH * sizeof(char8_t));
}

WifiSettingsEvent::WifiSettingsEvent(Deserializer& deserializer) {
    deserialize(deserializer);
}

WifiSettingsEvent::~WifiSettingsEvent() {
}

void WifiSettingsEvent::deserialize(Deserializer& deserializer) {
    uint8_t status;
    deserializer >> status;
    m_mode = static_cast<WifiMode>(status);
    for (auto i = 0U; i < MAX_SSID_LENGTH; i++) {
        deserializer >> m_ssid[i];
    }
    for (auto i = 0U; i < MAX_PWD_LENGTH; i++) {
        deserializer >> m_pwd[i];
    }
}

void WifiSettingsEvent::serialize(Serializer& serializer) const {
    serializer << static_cast<uint8_t>(m_mode);
    for (auto i = 0U; i < MAX_SSID_LENGTH; i++) {
        serializer << m_ssid[i];
    }
    for (auto i = 0U; i < MAX_PWD_LENGTH; i++) {
        serializer << m_pwd[i];
    }
}

void WifiSettingsEvent::setMode(const WifiMode mode) { 
    m_mode = mode; 
}

WifiMode WifiSettingsEvent::getMode() const { 
    return m_mode; 
}

void WifiSettingsEvent::setSSID(const char8_t ssid[MAX_SSID_LENGTH]) {       
    if (ssid != NULL) {
        mempcpy(&m_ssid[0], &ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
    } 
}

void WifiSettingsEvent::getSSID(char8_t ssid[MAX_SSID_LENGTH]) const { 
    if (ssid != NULL) {
        mempcpy(&ssid[0], &m_ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
    }
}

void WifiSettingsEvent::setPWD(const char8_t password[MAX_PWD_LENGTH]) {       
    if (password != NULL) {
        mempcpy(&m_pwd[0], &password[0], MAX_PWD_LENGTH * sizeof(char8_t));
    } 
}

void WifiSettingsEvent::getPWD(char8_t password[MAX_PWD_LENGTH]) const { 
    if (password != NULL) {
        mempcpy(&password[0], &m_pwd[0], MAX_PWD_LENGTH * sizeof(char8_t));
    }
}
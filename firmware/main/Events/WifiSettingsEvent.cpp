#include "WifiSettingsEvent.h"

WifiSettingsEvent::WifiSettingsEvent() :
    m_mode(WifiMode::STL) {
    memset(&m_pwd[0], 0, MAX_PWD_LENGTH * sizeof(char8_t));
    memset(&m_ssid[0], 0, MAX_SSID_LENGTH * sizeof(char8_t));
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
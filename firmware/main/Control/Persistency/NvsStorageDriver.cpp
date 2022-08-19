#include "NvsStorageDriver.h"

#include <HumiDevice.Serialization/Deserializer.h>
#include <HumiDevice.Serialization/Serializer.h>

#include "nvs.h"
#include "nvs_flash.h"

// checks given char* with expected property name with optional NULL-Termination
#define CHECK_PROP(name, expected) strncmp(name, expected, sizeof(expected) - 1) == 0

NvsStorageDriver::NvsStorageDriver() : 
    m_layout() {
}

NvsStorageDriver::~NvsStorageDriver(){
}

esp_err_t NvsStorageDriver::initialize() {
    esp_err_t err = m_layout.initialize();
    if (err != ESP_OK) {
        ESP_LOGE("NvsStorageDriver", "NVS Layout failure (%s)", esp_err_to_name(err));
        return ESP_ERR_FLASH_NOT_INITIALISED;
    }

    if (!checkVersion()) {
        ESP_LOGI("NvsStorageDriver", "Version mismatch, restored default values");
        return ESP_ERR_FLASH_NOT_INITIALISED;
    }

    return ESP_OK;
}

void NvsStorageDriver::readWifiConfig(WifiSettingsEvent* pWifiParam) {
    if (pWifiParam == NULL) return;

    const WifiMode mode = strlen(m_layout.ssid) > 0 ? WifiMode::STA : WifiMode::AP;

    pWifiParam->setSsid(m_layout.ssid);
    pWifiParam->setPassword(m_layout.password);
    pWifiParam->setMode(mode);
}

void NvsStorageDriver::readDeviceConfig(DeviceSettingsEvent* pDeviceParam) {
    if (pDeviceParam == NULL) return;
    
    pDeviceParam->setBrokerUri(m_layout.brokerUri);
    pDeviceParam->setSn(m_layout.sn);
    pDeviceParam->setRoom(m_layout.room);
    pDeviceParam->setInterval(m_layout.interval);
}

esp_err_t NvsStorageDriver::fetch() {
    return m_layout.fetch();
}

esp_err_t NvsStorageDriver::commit() {
    return m_layout.commit();
}

bool NvsStorageDriver::get(const char8_t* name, void** data, size_t* const size) {
    if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_SN)) {
        *data = m_layout.sn;
        *size = sizeof(m_layout.sn);
    }
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_ROOM)) {
        *data = m_layout.room;
        *size = sizeof(m_layout.room);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_INTERVAL)) {
        *data = &m_layout.interval;
        *size = sizeof(m_layout.interval);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_BROKER_URI)) {
        *data = m_layout.brokerUri;
        *size = sizeof(m_layout.brokerUri);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_SSID)) {
        *data = m_layout.ssid;
        *size = sizeof(m_layout.ssid);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_PASSPHRASE)) {
        *data = m_layout.password;
        *size = sizeof(m_layout.password);
    }
    else {
        ESP_LOGW("NvsStorageDriver", "Configuration '%s' is not supported by NVS Storage", name);
        return false;
    }
    ESP_LOGI("NvsStorageDriver", "Get %s = %s", name, *data);

    return true;
}
    
bool NvsStorageDriver::put(const char8_t* name, const void* data, size_t size) {
    void* dest = NULL;
    size_t capacity;

    if (!get(name, &dest, &capacity)) {
        return false;
    }

    ESP_LOGI("NvsStorageDriver", "Replace %s: %s = %s", name, dest, data);

    if (size > capacity) {
        ESP_LOGW("NvsStorageDriver", "Storage Size for Property %s is not big enough (%i < %i) ", name, size, capacity);
        size = capacity;
    }

    memcpy(dest, data, size);

    return true;
}

bool NvsStorageDriver::checkVersion() {
    if (m_layout.version == NvsLayout::INVALID_VERSION) {
        ESP_LOGW("NvsStorageDriver", "Invalid NVS Cfg Version! Restore default values.");

        m_layout.reset();
        return false;
    }
    else if (m_layout.version < NvsLayout::LAYOUT_VERSION) {
        ESP_LOGW("NvsStorageDriver", "Detected unsupported NVS Cfg Version %i. Update values to new version %i", m_layout.version, NvsLayout::LAYOUT_VERSION);
        m_layout.upgrade(m_layout.version, NvsLayout::LAYOUT_VERSION);

        return m_layout.commit() == ESP_OK;
    }
    else if (m_layout.version  == NvsLayout::LAYOUT_VERSION) {
        ESP_LOGI("NvsStorageDriver", "Detected NVS Cfg Version %i!", m_layout.version);
        return true;
    }
    else {
        ESP_LOGE("NvsStorageDriver", "Detected unsupported NVS Cfg Version %i. Can not downgrade to Version %i", m_layout.version, NvsLayout::LAYOUT_VERSION);
        m_layout.reset();

        return false;
    }
}

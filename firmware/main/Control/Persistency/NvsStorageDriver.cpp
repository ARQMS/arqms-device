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

    // update internal data
    m_layout.fetch();

    const WifiMode mode = strlen(m_layout.ssid) > 0 ? WifiMode::STA : WifiMode::AP;

    pWifiParam->setSsid(m_layout.ssid);
    pWifiParam->setPassword(m_layout.password);
    pWifiParam->setMode(mode);
}

void NvsStorageDriver::readDeviceConfig(DeviceSettingsEvent* pDeviceParam) {
    if (pDeviceParam == NULL) return;
    
    // update internal data
    m_layout.fetch();

    pDeviceParam->setBrokerUri(m_layout.brokerUri);
    pDeviceParam->setSn(m_layout.sn);
}

esp_err_t NvsStorageDriver::get(const char8_t* name, void** data, size_t* size) {
    esp_err_t err = m_layout.fetch();
    if(err != ESP_OK){
        return err;
    }

    if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_SN)) {
        *data = m_layout.sn;
        *size = sizeof(m_layout.sn);
    }
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_ROOM)) {
        // TODO
        static char8_t* ROOM = CONFIG_DEVICE_ROOM;
        *data = (void*)ROOM;
        *size = sizeof(ROOM);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_INTERVAL)) {
        // TODO
        static uint32_t INTERVAL = CONFIG_DEVICE_INTERVAL;
        *data = (void*)&INTERVAL;
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
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

    
esp_err_t NvsStorageDriver::put(const char8_t* name, const void* data, const size_t size) {
    if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_SN)) {
        // readonly, do not change serial number!
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_ROOM)) {
        // TODO 
        ESP_LOGI("NvsStorageDriver", "Write %s:%s", name, data);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_INTERVAL)) {
        // TODO 
        const uint32_t interval = *(const uint32_t*)data;
        ESP_LOGI("NvsStorageDriver", "Write %s:%i", name, interval);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_BROKER_URI)) {
        memcpy(m_layout.brokerUri, data, size);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_SSID)) {
        memcpy(m_layout.ssid, data, size);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_PASSPHRASE)) {
        memcpy(m_layout.password, data, size);
    } 
    else {
        ESP_LOGW("NvsStorageDriver", "Configuration '%s' is not supported by NVS Storage", name);
        return ESP_ERR_INVALID_ARG;
    } 

    return m_layout.commit();
}

bool NvsStorageDriver::checkVersion() {
    m_layout.fetch();

    if (m_layout.version == NvsLayout::INVALID_VERSION) {
        ESP_LOGW("NvsStorageDriver", "Invalid NVS Cfg Version! Restore default values.");

        m_layout.restoreDefault();
        return false;
    }
    else if (m_layout.version < NvsLayout::LAYOUT_VERSION) {
        ESP_LOGW("NvsStorageDriver", "Detected unsupported NVS Cfg Version %i. Update values to new version %i", m_layout.version, NvsLayout::LAYOUT_VERSION);

        m_layout.version = NvsLayout::LAYOUT_VERSION;
        // TODO update layout

        return true;
    }
    else if (m_layout.version  == NvsLayout::LAYOUT_VERSION) {
        ESP_LOGI("NvsStorageDriver", "Detected NVS Cfg Version %i!", m_layout.version);
        return true;
    }
    else {
        ESP_LOGE("NvsStorageDriver", "Detected unsupported NVS Cfg Version %i. Can not downgrade to Version %i", m_layout.version, NvsLayout::LAYOUT_VERSION);
        m_layout.restoreDefault();

        return false;
    }
}

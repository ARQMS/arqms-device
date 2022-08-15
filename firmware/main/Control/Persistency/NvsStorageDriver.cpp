#include "NvsStorageDriver.h"

#include <HumiDevice.Serialization/Deserializer.h>
#include <HumiDevice.Serialization/Serializer.h>

#include "nvs.h"
#include "nvs_flash.h"

const char8_t* NvsStorageDriver::s_partitionNamespace   = "Settings";
const char8_t* NvsStorageDriver::s_wifiConfigKey        = "WifiConfig";
const char8_t* NvsStorageDriver::s_deviceConfigKey      = "DeviceConfig";
const char8_t* NvsStorageDriver::s_versionKey           = "Version";

// checks given char* with expected property name with optional NULL-Termination
#define CHECK_PROP(name, expected) strncmp(name, expected, sizeof(expected) - 1) == 0

NvsStorageDriver::NvsStorageDriver() {
}

NvsStorageDriver::~NvsStorageDriver(){
}

esp_err_t NvsStorageDriver::initialize() {
    esp_err_t err = nvs_flash_init();

    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW("NvsStorageDriver", "Failed initialize NVS. No free pages or new version detected");
        return err;
    }

    if (!checkVersion()) {
        ESP_LOGI("NvsStorageDriver", "Version mismatch, restore default values");
        restoreDefault();
    }

    return ESP_OK;
}

esp_err_t NvsStorageDriver::restoreDefault() {
    const WifiMode mode = strlen(CONFIG_WIFI_SSID) > 0 ? WifiMode::STA : WifiMode::AP;

    WifiSettingsEvent wifiSettings(mode, CONFIG_WIFI_SSID, CONFIG_WIFI_PASSWORD);
    DeviceSettingsEvent deviceSettings(CONFIG_DEVICE_BROKER, CONFIG_DEVICE_SN);  
    // TODO room CONFIG_ESP_DEVICE_ROOM
    // TODO interval  CONFIG_ESP_DEVICE_INTERVAL
    
    // update version
    nvs_handle_t nvsHandle;
    checkEspError(nvs_open(s_partitionNamespace, NVS_READWRITE, &nvsHandle));
    checkEspError(nvs_set_i8(nvsHandle, s_versionKey, LAYOUT_VERSION));
    nvs_close(nvsHandle);

    storeData<WifiSettingsEvent>(wifiSettings, s_wifiConfigKey);
    storeData<DeviceSettingsEvent>(deviceSettings, s_deviceConfigKey);

    return ESP_OK;
}

void NvsStorageDriver::readWifiConfig(WifiSettingsEvent* pWifiParam) {
    if (pWifiParam == NULL) return;

    loadData<WifiSettingsEvent>(*pWifiParam, s_wifiConfigKey);
}

void NvsStorageDriver::readDeviceConfig(DeviceSettingsEvent* pDeviceParam) {
    if (pDeviceParam == NULL) return;

    loadData<DeviceSettingsEvent>(*pDeviceParam, s_deviceConfigKey);
}

esp_err_t NvsStorageDriver::readConfiguration(const char8_t* name, void** data, size_t* size) {
    // TODO read from NVS, following code is for demonstration purpose
	esp_err_t err = ESP_OK;
	
    if(err != ESP_OK){
        return err;
    }

    // TODO read from NVS
    /*if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_SN)) {
        DeviceSettingsEvent msg;
        loadData<DeviceSettingsEvent>(msg, s_deviceConfigKey);

        msg.getSn(*data);
        //*data = (void*)sn;
        //*size = sizeof(sn);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_ROOM)) {
        *data = (void*)room;
        *size = sizeof(room);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_INTERVAL)) {
        *data = (void*)&interval;
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_BROKER_URI)) {
        *data = (void*)broker;
        *size = sizeof(broker);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_SSID)) {
        *data = (void*)wifi_ssid;
        *size = sizeof(wifi_ssid);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_PASSPHRASE)) {
        *data = (void*)wifi_pwd;
        *size = sizeof(wifi_pwd);
    } */
    else {
        ESP_LOGW("HumiDevice", "Configuration '%s' is not supported by NVS Storage", name);
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

    
esp_err_t NvsStorageDriver::writeConfiguration(const char8_t* name, const void* data, const size_t size) {
    // TODO write to NVS
    esp_err_t err = ESP_OK;

    if(err != ESP_OK){
        return err;
    }

    if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_SN)) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_ROOM)) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_INTERVAL)) {
        const uint32_t interval = *(const uint32_t*)data;
        ESP_LOGI("HumiDevice", "Write %s:%i", name, interval);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_BROKER_URI)) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_SSID)) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (CHECK_PROP(name, ESP_CTRL_PROP_WIFI_PASSPHRASE)) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else {
        ESP_LOGW("HumiDevice", "Configuration '%s' is not supported by NVS Storage", name);
        return ESP_ERR_INVALID_ARG;
    } 

    return ESP_OK;
}

esp_err_t NvsStorageDriver::storeData(SerializableIfc& data, const char8_t* key, uint8_t* buffer, const size_t size) {
    Serializer serializer(buffer, size);
    data.serialize(serializer);

    nvs_handle_t nvsHandle;
    checkEspError(nvs_open(s_partitionNamespace, NVS_READWRITE, &nvsHandle));
    checkEspError(nvs_set_blob(nvsHandle, key, &buffer, serializer.getBufferPos()));
    checkEspError(nvs_commit(nvsHandle));

    nvs_close(nvsHandle);

    return ESP_OK;
}

esp_err_t NvsStorageDriver::loadData(SerializableIfc& data, const char8_t* key, uint8_t* buffer, size_t* size) const {
    nvs_handle_t nvsHandle;
    checkEspError(nvs_open(s_partitionNamespace, NVS_READWRITE, &nvsHandle));
    checkEspError(nvs_get_blob(nvsHandle, key, buffer, size));

    Deserializer deserializer(buffer, *size);
    data.deserialize(deserializer);
    
    nvs_close(nvsHandle);

    return ESP_OK;
}

bool NvsStorageDriver::checkVersion() const {
    nvs_handle_t nvsHandle;
    int8_t versionInFlash = -1;
    checkEspError(nvs_open(s_partitionNamespace, NVS_READWRITE, &nvsHandle));
    nvs_get_i8(nvsHandle, s_versionKey, &versionInFlash);
    nvs_close(nvsHandle);

    return versionInFlash == LAYOUT_VERSION;
}

bool NvsStorageDriver::checkEspError(const esp_err_t erroCode) const  {
    if (erroCode != ESP_OK) {
        ESP_LOGE("NvsStorageDriver", "Error in NvsStorageDriver: %s", esp_err_to_name(erroCode));
        return false;
    }

    return true;
}
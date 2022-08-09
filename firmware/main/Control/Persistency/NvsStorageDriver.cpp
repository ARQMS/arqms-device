#include "NvsStorageDriver.h"

#include <HumiDevice.Serialization/Deserializer.h>
#include <HumiDevice.Serialization/Serializer.h>

// TODO remove demo. Use struct or NVS with fixed length strings to avvoid dynamic memory!
WifiSettingsEvent g_wifiSettings;
DeviceSettingsEvent g_deviceSettings;

static char8_t sn[DeviceSettingsEvent::MAX_SN_LENGTH]               = "AR-0001";
static char8_t room[32]                                             = "OfficeRoom1";
static char8_t broker[DeviceSettingsEvent::MAX_BROKER_URI_LENGTH]   = "mqtt://192.168.0.16:8883";
static char8_t wifi_ssid[WifiSettingsEvent::MAX_SSID_LENGTH]        = "";
static char8_t wifi_pwd[WifiSettingsEvent::MAX_PWD_LENGTH]          = "";
static int  interval                                                = 15;

// checks given char* with expected property name with optional NULL-Termination
#define CHECK_PROP(name, expected) strncmp(name, expected, sizeof(expected) - 1) == 0

NvsStorageDriver::NvsStorageDriver() {

}

NvsStorageDriver::~NvsStorageDriver() {
    
}

esp_err_t NvsStorageDriver::initialize() {
    const WifiMode mode = strnlen(wifi_ssid, sizeof(wifi_ssid)) > 0 ? WifiMode::STA : WifiMode::AP;
    g_wifiSettings.setSsid(wifi_ssid);
    g_wifiSettings.setPassword(wifi_pwd);
    g_wifiSettings.setMode(mode);

    g_deviceSettings.setBrokerUri(broker);
    g_deviceSettings.setSn(sn);

    return ESP_OK;
}

void NvsStorageDriver::readWifiConfig(WifiSettingsEvent* pWifiParam) {
    if (pWifiParam == NULL) return;

    // TODO use blobs in NVS system to deserialize binary data
    static uint8_t binaryBlob[sizeof(WifiSettingsEvent)];
    Serializer serializer(binaryBlob, sizeof(WifiSettingsEvent));
    g_wifiSettings.serialize(serializer);

    Deserializer deserializer(binaryBlob, sizeof(WifiSettingsEvent));
    pWifiParam->deserialize(deserializer);
}

void NvsStorageDriver::readDeviceConfig(DeviceSettingsEvent* pDeviceParam) {
    if (pDeviceParam == NULL) return;

    // TODO use blobs in NVS system to deserialize binary data
    static uint8_t binaryBlob[sizeof(DeviceSettingsEvent)];
    Serializer serializer(binaryBlob, sizeof(DeviceSettingsEvent));
    g_deviceSettings.serialize(serializer);

    Deserializer deserializer(binaryBlob, sizeof(DeviceSettingsEvent));
    pDeviceParam->deserialize(deserializer);
}

esp_err_t NvsStorageDriver::readConfiguration(const char8_t* name, void** data, size_t* size) {
    // TODO read from NVS, following code is for demonstration purpose

    if (CHECK_PROP(name, ESP_CTRL_PROP_DEVICE_SN)) {
        *data = (void*)sn;
        *size = sizeof(sn);
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
    } 
    else {
        ESP_LOGW("HumiDevice", "Configuration '%s' is not supported by NVS Storage", name);
        return ESP_ERR_INVALID_ARG;
    } 
    
    return ESP_OK;
}

esp_err_t NvsStorageDriver::writeConfiguration(const char8_t* name, const void* data, const size_t size) {
    // TODO write to NVS

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

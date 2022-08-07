#include "NvsStorageDriver.h"

// TODO remove demo. Use struct or NVS with fixed length strings to avvoid dynamic memory!
static char8_t sn[32]           = "AR-0001";
static char8_t room[32]         = "OfficeRoom1";
static char8_t broker[64]       = "https://192.168.0.16";
static char8_t wifi_ssid[32]    = "SSID";
static char8_t wifi_pwd[64]     = "PWD";
static int  interval            = 15;

// checks given char* with expected property name with optional NULL-Termination
#define CHECK_PROP(name, expected) strncmp(name, expected, sizeof(expected) - 1) == 0

NvsStorageDriver::NvsStorageDriver() {

}

NvsStorageDriver::~NvsStorageDriver() {
    
}

void NvsStorageDriver::readWifiConfig(WifiParameters* pWifiParam) {
    if (pWifiParam == NULL) return;

    strncpy(pWifiParam->ssid, wifi_ssid, sizeof(pWifiParam->ssid));
    strncpy(pWifiParam->password, wifi_pwd, sizeof(pWifiParam->password));
}

esp_err_t NvsStorageDriver::readConfiguration(const char* name, void** data, size_t* size) {
    // TODO read from NVS

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

esp_err_t NvsStorageDriver::writeConfiguration(const char* name, const void* data, const size_t size) {
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

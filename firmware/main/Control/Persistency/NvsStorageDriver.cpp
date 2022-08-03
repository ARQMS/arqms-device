#include "NvsStorageDriver.h"

// TODO remove demo. Use struct and fixed length strings!
static const char sn[]           = "AR-0001";
static const char room[]         = "OfficeRoom1";
static const char broker[]       = "https://192.168.0.16";
static const char wifi_ssid[]    = "SSID";
static const char wiif_pwd[]     = "PWD";
static const int  interval       = 15;

NvsStorageDriver::NvsStorageDriver() {

}

NvsStorageDriver::~NvsStorageDriver() {
    
}

esp_err_t NvsStorageDriver::readConfiguration(const char* name, void** data, size_t* size) {
    // TODO read from NVS

    if (strncmp(name, ESP_CTRL_PROP_DEVICE_SN, sizeof(ESP_CTRL_PROP_DEVICE_SN) - 1) == 0) {
        *data = (void*)sn;
        *size = sizeof(sn);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_DEVICE_ROOM, sizeof(ESP_CTRL_PROP_DEVICE_ROOM) - 1) == 0) {
        *data = (void*)room;
        *size = sizeof(room);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_DEVICE_INTERVAL, sizeof(ESP_CTRL_PROP_DEVICE_INTERVAL) - 1) == 0) {
        *data = (void*)&interval;
    } 
    else if (strncmp(name, ESP_CTRL_PROP_DEVICE_BROKER_URI, sizeof(ESP_CTRL_PROP_DEVICE_BROKER_URI) - 1) == 0) {
        *data = (void*)broker;
        *size = sizeof(broker);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_WIFI_SSID, sizeof(ESP_CTRL_PROP_WIFI_SSID) - 1) == 0) {
        *data = (void*)wifi_ssid;
        *size = sizeof(wifi_ssid);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_WIFI_PASSPHRASE, sizeof(ESP_CTRL_PROP_WIFI_PASSPHRASE) - 1) == 0) {
        *data = (void*)wiif_pwd;
        *size = sizeof(wiif_pwd);
    } 
    else {
        ESP_LOGW("HumiDevice", "Configuration '%s' is not supported by NVS Storage", name);
        return ESP_ERR_INVALID_ARG;
    } 
    
    return ESP_OK;
}

esp_err_t NvsStorageDriver::writeConfiguration(const char* name, void* data, size_t size) {
    // TODO write from NVS

    if (strncmp(name, ESP_CTRL_PROP_DEVICE_SN, sizeof(ESP_CTRL_PROP_DEVICE_SN) - 1) == 0) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_DEVICE_ROOM, sizeof(ESP_CTRL_PROP_DEVICE_ROOM) - 1) == 0) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_DEVICE_INTERVAL, sizeof(ESP_CTRL_PROP_DEVICE_INTERVAL) - 1) == 0) {
        ESP_LOGI("HumiDevice", "Write %s:%i", name, data);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_DEVICE_BROKER_URI, sizeof(ESP_CTRL_PROP_DEVICE_BROKER_URI) - 1) == 0) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_WIFI_SSID, sizeof(ESP_CTRL_PROP_WIFI_SSID) - 1) == 0) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else if (strncmp(name, ESP_CTRL_PROP_WIFI_PASSPHRASE, sizeof(ESP_CTRL_PROP_WIFI_PASSPHRASE) - 1) == 0) {
        ESP_LOGI("HumiDevice", "Write %s:%s", name, data);
    } 
    else {
        ESP_LOGW("HumiDevice", "Configuration '%s' is not supported by NVS Storage", name);
        return ESP_ERR_INVALID_ARG;
    } 
    
    return ESP_OK;
}

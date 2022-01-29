#include "DeviceStorage.h"

#include "esp_wifi.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "HumiConfStore"

DeviceStorage::DeviceStorage() :
    initialized(false) {
}

DeviceStorage::~DeviceStorage() {
}

void DeviceStorage::initialize() {
    auto error = nvs_flash_init();
    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS parition was truncated and needs to be erased
        nvs_flash_erase_partition(STORAGE_NAMESPACE);

        // try again to init nvs
        error = nvs_flash_init();
    }
    ESP_ERROR_CHECK(error);    

    initialized = true;
}

bool DeviceStorage::isInitialized() {
    return initialized;
}


DeviceParameters DeviceStorage::getDeviceParameters(void) {
    NvsLayoutV1 nvsValues;

    // TODO: not generic enough for other configuration parts!
    // understand how NVS is organized to create maching configuration structure
    // with versioning

    nvs_handle* pHandler = nullptr;
    nvs_open(STORAGE_NAMESPACE, NVS_READONLY, pHandler);

    size_t size;
    nvs_get_blob(*pHandler, "Config", &nvsValues, &size);
    nvs_close(*pHandler);

    return nvsValues.deviceParameters;
}

void DeviceStorage::setDeviceParameters(const DeviceParameters& param) {
    NvsLayoutV1 nvsValues;
    nvsValues.deviceParameters = param;

    // TODO see getDeviceParameters

    nvs_handle* pHandler = nullptr;
    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, pHandler);

    nvs_set_blob(*pHandler, "Config", &nvsValues, CURRENT_LAYOUT_SIZE_BYTES);
    nvs_close(*pHandler);
}

WifiParameters DeviceStorage::getWifiParameters() {
    // IDF already supports wifi NVS storage
    wifi_config_t wifiConfig;
    esp_wifi_get_config(wifi_interface_t::WIFI_IF_STA, &wifiConfig);

    return static_cast<WifiParameters>(wifiConfig);
}

void DeviceStorage::setWifiParameters(const WifiParameters& param) {
    // IDF already supports wifi NVS storage
    wifi_config_t config = static_cast<wifi_config_t>(param);
    esp_wifi_set_config(wifi_interface_t::WIFI_IF_STA, &config);
}
    

void DeviceStorage::factoryDefault() {
    // TODO preserve DeviceParameter
    // nvs_flash_erase();

    // Write CURRENT_LAYOUT_VERSION
    // Write DeviceParameter
    // Write default values.
}
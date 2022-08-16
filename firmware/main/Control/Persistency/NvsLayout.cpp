#include "NvsLayout.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_partition.h"
#include "sdkconfig.h"

const char8_t* g_partitionNamespace   = "cfg";

// IDF supports up to 16 byte key!
const char8_t* g_versionKey           = "version";
const char8_t* g_wifiSsidKey          = "wifi.ssid";
const char8_t* g_wifiPasswordKey      = "wifi.pwd";
const char8_t* g_deviceSnKey          = "device.sn";
const char8_t* g_deviceBrokerKey      = "device.broker";

esp_err_t NvsLayout::initialize() {
    esp_err_t err = nvs_flash_init();

    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW("NvsStorageDriver", "Failed initialize NVS. No free pages or new version detected, flash erased. Restart device!");
        esp_partition_iterator_t it;
        it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

        // Loop through all matching partitions, in this case, all with the type 'data' until partition with desired 
        // label is found. Verify if its the same instance as the one found before.
        for (; it != NULL; it = esp_partition_next(it)) {
            const esp_partition_t *part = esp_partition_get(it);
            nvs_flash_erase_partition(part->label);
            ESP_LOGI("NvsStorageDriver", "\tfound partition '%s' at offset 0x%x with size 0x%x. Erased!", part->label, part->address, part->size);
        }

        // Release the partition iterator to release memory allocated for it
        esp_partition_iterator_release(it);

        // initialize again
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    return nvs_flash_init_partition(g_partitionNamespace);
}

esp_err_t NvsLayout::restoreDefault() {
    nvs_flash_erase_partition(g_partitionNamespace);

    version = LAYOUT_VERSION;
    memcpy(ssid, CONFIG_WIFI_SSID, sizeof(CONFIG_WIFI_SSID));
    memcpy(password, CONFIG_WIFI_PASSWORD, sizeof(CONFIG_WIFI_PASSWORD));
    memcpy(sn, CONFIG_DEVICE_SN, sizeof(CONFIG_DEVICE_SN));
    memcpy(brokerUri, CONFIG_DEVICE_BROKER, sizeof(CONFIG_DEVICE_BROKER));

    return commit();
}

esp_err_t NvsLayout::commit() {
    setU8(g_versionKey, version);
    setString(g_wifiSsidKey, ssid);
    setString(g_wifiPasswordKey, password);
    setString(g_deviceSnKey, sn);
    setString(g_deviceBrokerKey, brokerUri);

    return ESP_OK;
}

esp_err_t NvsLayout::fetch() {
    getU8(g_versionKey, &version, LAYOUT_VERSION);
    getString(g_wifiSsidKey, ssid, CONFIG_WIFI_SSID, sizeof(CONFIG_WIFI_SSID));
    getString(g_wifiPasswordKey, password, CONFIG_WIFI_PASSWORD, sizeof(CONFIG_WIFI_PASSWORD));
    getString(g_deviceSnKey, sn, CONFIG_DEVICE_SN, sizeof(CONFIG_DEVICE_SN));
    getString(g_deviceBrokerKey, brokerUri, CONFIG_DEVICE_BROKER, sizeof(CONFIG_DEVICE_BROKER));

    return ESP_OK;
}

void NvsLayout::setString(const char8_t* key, const char8_t* value) {
    nvs_handle_t nvsHandle;

    ESP_LOGI("NvsStorageDriver", "Set %s = %s", key, value);

    nvs_flash_init_partition(g_partitionNamespace);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_open_from_partition(g_partitionNamespace, key, NVS_READWRITE, &nvsHandle));
    nvs_set_str(nvsHandle, key, value);
    nvs_commit(nvsHandle);
    nvs_close(nvsHandle);
}

void NvsLayout::getString(const char8_t* key, char8_t* value, const char8_t* defaultVal, const size_t defaultLength) {
    nvs_handle_t nvsHandle;
    size_t size;

    nvs_flash_init_partition(g_partitionNamespace);
    nvs_open_from_partition(g_partitionNamespace, key, NVS_READWRITE, &nvsHandle);
    if (nvs_get_str(nvsHandle, key, value, &size) != ESP_OK) {
        memcpy(value, defaultVal, defaultLength);
    }

    ESP_LOGI("NvsStorageDriver", "Get %s = %s", key, value);

    nvs_close(nvsHandle);
}

void NvsLayout::setU8(const char8_t* key, const uint8_t value) {
    nvs_handle_t nvsHandle;

    nvs_flash_init_partition(g_partitionNamespace);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_open_from_partition(g_partitionNamespace, key, NVS_READWRITE, &nvsHandle));
    nvs_set_u8(nvsHandle, key, value);
    nvs_commit(nvsHandle);
    nvs_close(nvsHandle);
}

void NvsLayout::getU8(const char8_t* key, uint8_t* value, const uint8_t defaultVal) {
    nvs_handle_t nvsHandle;

    nvs_flash_init_partition(g_partitionNamespace);
    nvs_open_from_partition(g_partitionNamespace, key, NVS_READWRITE, &nvsHandle);
    if (nvs_get_u8(nvsHandle, key, value) != ESP_OK) {
        *value = defaultVal;
    }

    nvs_close(nvsHandle);
}

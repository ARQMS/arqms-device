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
const char8_t* g_deviceSnKey          = "dev.sn";
const char8_t* g_deviceBrokerKey      = "dev.broker";
const char8_t* g_roomKey              = "dev.room";

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

    ESP_ERROR_CHECK(nvs_flash_init_partition(g_partitionNamespace));

    return fetch();
}

esp_err_t NvsLayout::reset() {
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_flash_erase_partition(g_partitionNamespace));
    ESP_ERROR_CHECK(nvs_flash_init_partition(g_partitionNamespace));
    return fetch();
}

esp_err_t NvsLayout::upgrade(const NvsVersion_t oldVersion, const NvsVersion_t newVersion) {
    if (oldVersion >= newVersion) {
        // nothing to do
        return ESP_ERR_INVALID_ARG;
    }

    version = newVersion;

    // implement any layout changes for backward compatibility

    return ESP_OK;
}

esp_err_t NvsLayout::commit() {
    ESP_LOGI("NvsLayout", "Commit partition %s V%i", g_partitionNamespace, version);

    setU8(g_versionKey, version);
    setString(g_deviceSnKey, sn);
    setString(g_deviceBrokerKey, brokerUri);
    setString(g_roomKey, room);
    //< TODO setU32(g_intervalKey, interval);
    setString(g_wifiSsidKey, ssid);
    setString(g_wifiPasswordKey, password);

    return ESP_OK;
}

esp_err_t NvsLayout::fetch() {
    ESP_LOGI("NvsLayout", "Fetch partition %s", g_partitionNamespace);

    getU8(g_versionKey, &version, LAYOUT_VERSION);
    getString(g_deviceSnKey, sn, sizeof(sn), CONFIG_DEVICE_SN, sizeof(CONFIG_DEVICE_SN));
    getString(g_deviceBrokerKey, brokerUri, sizeof(brokerUri), CONFIG_DEVICE_BROKER, sizeof(CONFIG_DEVICE_BROKER));
    getString(g_roomKey, room, sizeof(room), CONFIG_DEVICE_ROOM, sizeof(CONFIG_DEVICE_ROOM));
    //< TODO getU32(g_intervalKey, interval, CONFIG_DEVICE_INTERVAL);
    getString(g_wifiSsidKey, ssid, sizeof(ssid), CONFIG_WIFI_SSID, sizeof(CONFIG_WIFI_SSID));
    getString(g_wifiPasswordKey, password, sizeof(password), CONFIG_WIFI_PASSWORD, sizeof(CONFIG_WIFI_PASSWORD));

    return ESP_OK;
}

void NvsLayout::setString(const char8_t* key, const char8_t* value) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(g_partitionNamespace, key, NVS_READWRITE, &nvsHandle);
    nvs_set_str(nvsHandle, key, value);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvsHandle));
    nvs_close(nvsHandle);

    ESP_LOGI("NvsLayout", "\t%s: %s", key, value);
}

void NvsLayout::getString(const char8_t* key, char8_t* value, const size_t valueLength, const char8_t* defaultVal, const size_t defaultLength) {
    nvs_handle_t nvsHandle;
    size_t size = valueLength;

    // reset content in variable
    memset(value, 0, valueLength);

    nvs_open_from_partition(g_partitionNamespace, key, NVS_READONLY, &nvsHandle);
    if (nvs_get_str(nvsHandle, key, value, &size) != ESP_OK) {
        ESP_LOGI("NvsLayout", "\t%s was restored to %s", key, defaultVal);
        memcpy(value, defaultVal, defaultLength);
    } else {
        ESP_LOGI("NvsLayout", "\t%s: %s", key, value);
    }

    nvs_close(nvsHandle);
}

void NvsLayout::setU8(const char8_t* key, const uint8_t value) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(g_partitionNamespace, key, NVS_READWRITE, &nvsHandle);
    nvs_set_u8(nvsHandle, key, value);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvsHandle));
    nvs_close(nvsHandle);

    ESP_LOGI("NvsLayout", "\t%s: %i", key, value);
}

void NvsLayout::getU8(const char8_t* key, uint8_t* value, const uint8_t defaultVal) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(g_partitionNamespace, key, NVS_READONLY, &nvsHandle);
    if (nvs_get_u8(nvsHandle, key, value) != ESP_OK) {
        ESP_LOGI("NvsLayout", "\t%s was restored to %i", key, defaultVal);
        *value = defaultVal;
    } else {
        ESP_LOGI("NvsLayout", "\t%s: %i", key, *value);
    }

    nvs_close(nvsHandle);
}

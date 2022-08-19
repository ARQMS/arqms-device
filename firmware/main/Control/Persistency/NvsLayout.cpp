#include "NvsLayout.h"
#include "NvsUtil.h"

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_partition.h"
#include "sdkconfig.h"

// IDF supports up to 16 byte key!
const char8_t* g_versionKey           = "version";
const char8_t* g_wifiSsidKey          = "wifi.ssid";
const char8_t* g_wifiPasswordKey      = "wifi.pwd";
const char8_t* g_deviceSnKey          = "dev.sn";
const char8_t* g_deviceBrokerKey      = "dev.broker";
const char8_t* g_roomKey              = "dev.room";
const char8_t* g_intervalKey          = "dev.interval";

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

    ESP_ERROR_CHECK(nvs_flash_init_partition(NvsUtil::PARTITION_NAMESPACE));

    return fetch();
}

esp_err_t NvsLayout::reset() {
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_flash_erase_partition(NvsUtil::PARTITION_NAMESPACE));
    ESP_ERROR_CHECK(nvs_flash_init_partition(NvsUtil::PARTITION_NAMESPACE));
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
    ESP_LOGI("NvsLayout", "Commit partition %s V%i", NvsUtil::PARTITION_NAMESPACE, version);

    NvsUtil::setU8(g_versionKey, version);
    NvsUtil::setString(g_deviceSnKey, sn);
    NvsUtil::setString(g_deviceBrokerKey, brokerUri);
    NvsUtil::setString(g_roomKey, room);
    NvsUtil::setU32(g_intervalKey, interval);
    NvsUtil::setString(g_wifiSsidKey, ssid);
    NvsUtil::setString(g_wifiPasswordKey, password);

    return ESP_OK;
}

esp_err_t NvsLayout::fetch() {
    ESP_LOGI("NvsLayout", "Fetch partition %s", NvsUtil::PARTITION_NAMESPACE);

    NvsUtil::getU8(g_versionKey, &version, LAYOUT_VERSION);
    NvsUtil::getString(g_deviceSnKey, sn, sizeof(sn), CONFIG_DEVICE_SN, sizeof(CONFIG_DEVICE_SN));
    NvsUtil::getString(g_deviceBrokerKey, brokerUri, sizeof(brokerUri), CONFIG_DEVICE_BROKER, sizeof(CONFIG_DEVICE_BROKER));
    NvsUtil::getString(g_roomKey, room, sizeof(room), CONFIG_DEVICE_ROOM, sizeof(CONFIG_DEVICE_ROOM));
    NvsUtil::getU32(g_intervalKey, &interval, CONFIG_DEVICE_INTERVAL);
    NvsUtil::getString(g_wifiSsidKey, ssid, sizeof(ssid), CONFIG_WIFI_SSID, sizeof(CONFIG_WIFI_SSID));
    NvsUtil::getString(g_wifiPasswordKey, password, sizeof(password), CONFIG_WIFI_PASSWORD, sizeof(CONFIG_WIFI_PASSWORD));

    return ESP_OK;
}

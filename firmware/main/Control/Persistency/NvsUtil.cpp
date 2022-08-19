#include "NvsUtil.h"

#include "nvs.h"
#include "nvs_flash.h"

const char8_t* NvsUtil::PARTITION_NAMESPACE = "cfg";

void NvsUtil::setString(const char8_t* key, const char8_t* value) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(PARTITION_NAMESPACE, key, NVS_READWRITE, &nvsHandle);
    nvs_set_str(nvsHandle, key, value);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvsHandle));
    nvs_close(nvsHandle);

    ESP_LOGI("NvsUtil", "\t%s: %s", key, value);
}

void NvsUtil::getString(const char8_t* key, char8_t* value, const size_t valueLength, const char8_t* defaultVal, const size_t defaultLength) {
    nvs_handle_t nvsHandle;
    size_t size = valueLength;

    // reset content in variable
    memset(value, 0, valueLength);

    nvs_open_from_partition(PARTITION_NAMESPACE, key, NVS_READONLY, &nvsHandle);
    if (nvs_get_str(nvsHandle, key, value, &size) != ESP_OK) {
        ESP_LOGI("NvsUtil", "\t%s was restored to %s", key, defaultVal);
        memcpy(value, defaultVal, defaultLength);
    } else {
        ESP_LOGI("NvsUtil", "\t%s: %s", key, value);
    }

    nvs_close(nvsHandle);
}

void NvsUtil::setU8(const char8_t* key, const uint8_t value) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(PARTITION_NAMESPACE, key, NVS_READWRITE, &nvsHandle);
    nvs_set_u8(nvsHandle, key, value);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvsHandle));
    nvs_close(nvsHandle);

    ESP_LOGI("NvsUtil", "\t%s: %i", key, value);
}

void NvsUtil::getU8(const char8_t* key, uint8_t* value, const uint8_t defaultVal) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(PARTITION_NAMESPACE, key, NVS_READONLY, &nvsHandle);
    if (nvs_get_u8(nvsHandle, key, value) != ESP_OK) {
        ESP_LOGI("NvsUtil", "\t%s was restored to %i", key, defaultVal);
        *value = defaultVal;
    } else {
        ESP_LOGI("NvsUtil", "\t%s: %i", key, *value);
    }

    nvs_close(nvsHandle);
}

void NvsUtil::setU32(const char8_t* key, const uint32_t value) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(PARTITION_NAMESPACE, key, NVS_READWRITE, &nvsHandle);
    nvs_set_u32(nvsHandle, key, value);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_commit(nvsHandle));
    nvs_close(nvsHandle);

    ESP_LOGI("NvsUtil", "\t%s: %i", key, value);
}

void NvsUtil::getU32(const char8_t* key, uint32_t* value, const uint32_t defaultVal) {
    nvs_handle_t nvsHandle;

    nvs_open_from_partition(PARTITION_NAMESPACE, key, NVS_READONLY, &nvsHandle);
    if (nvs_get_u32(nvsHandle, key, value) != ESP_OK) {
        ESP_LOGI("NvsUtil", "\t%s was restored to %i", key, defaultVal);
        *value = defaultVal;
    } else {
        ESP_LOGI("NvsUtil", "\t%s: %i", key, *value);
    }

    nvs_close(nvsHandle);
}

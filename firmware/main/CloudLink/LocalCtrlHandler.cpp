#include "LocalCtrlHandler.h"

#include <mdns.h>
#include <esp_https_server.h>

#define ESP_CTRL_NUMBER_OF_PROPERTIES       6
// mDNS does not work at the moment, so we use IP
#define ESP_CTRL_SERVICE_NAME               "192.168.4.1"

ConfigProviderIfc* LocalCtrlHandler::s_pNvsStorageDriver = NULL;

LocalCtrlHandler::LocalCtrlHandler() {
}

esp_err_t LocalCtrlHandler::startService(void) {
    httpd_ssl_config_t httpsCfg = HTTPD_SSL_CONFIG_DEFAULT();

    // Load server certificate
    extern const unsigned char cacertPemStart[] asm("_binary_cacert_pem_start");
    extern const unsigned char cacertPemEnd[]   asm("_binary_cacert_pem_end");
    httpsCfg.cacert_pem = cacertPemStart;
    httpsCfg.cacert_len = cacertPemEnd - cacertPemStart;

    // Load server private key
    extern const unsigned char prvtkeyPemStart[] asm("_binary_prvtkey_pem_start");
    extern const unsigned char prvtkeyPemEnd[]   asm("_binary_prvtkey_pem_end");
    httpsCfg.prvtkey_pem = prvtkeyPemStart;
    httpsCfg.prvtkey_len = prvtkeyPemEnd - prvtkeyPemStart;

    esp_local_ctrl_config_t config = {
        .transport          = ESP_LOCAL_CTRL_TRANSPORT_HTTPD,
        .transport_config = {
            .httpd          = &httpsCfg
        },
        .proto_sec = {
            .version        = PROTOCOM_SEC0,
            .custom_handle  = NULL,
            .pop            = NULL,
        },
        .handlers = {
            .get_prop_values = &LocalCtrlHandler::getPropertyValues,
            .set_prop_values = &LocalCtrlHandler::setPropertyValues,
            .usr_ctx         = s_pNvsStorageDriver,
            .usr_ctx_free_fn = NULL
        },
        // Maximum number of properties that may be set
        .max_properties      = ESP_CTRL_NUMBER_OF_PROPERTIES
    };

    // setup DNS - hostname
   mdns_init();
   mdns_hostname_set(ESP_CTRL_SERVICE_NAME);

    // start service
    const esp_err_t err = esp_local_ctrl_start(&config);

    // register properties after service is started
    registerProperty(ESP_CTRL_PROP_DEVICE_SN, PropertyType::PROP_TYPE_CHAR_STRING, true);
    registerProperty(ESP_CTRL_PROP_DEVICE_ROOM, PropertyType::PROP_TYPE_CHAR_STRING);
    registerProperty(ESP_CTRL_PROP_DEVICE_INTERVAL, PropertyType::PROP_TYPE_UINT32);
    registerProperty(ESP_CTRL_PROP_DEVICE_BROKER_URI, PropertyType::PROP_TYPE_CHAR_STRING);
    registerProperty(ESP_CTRL_PROP_WIFI_SSID, PropertyType::PROP_TYPE_CHAR_STRING);
    registerProperty(ESP_CTRL_PROP_WIFI_PASSPHRASE, PropertyType::PROP_TYPE_CHAR_STRING);
    
    return err;
}

esp_err_t LocalCtrlHandler::stopService() {
    return esp_local_ctrl_stop();
}

void LocalCtrlHandler::registerProperty(const char* name, const PropertyType type, const bool isReadOnly) {
    PropertyFlags flags = isReadOnly ? PropertyFlags::PROP_FLAG_READONLY : PropertyFlags::NONE;

    const esp_local_ctrl_prop_t property = {
        .name        = const_cast<char*>(name),
        .type        = type,
        .size        = type.getSize(),
        .flags       = static_cast<uint32_t>(flags),
        .ctx         = NULL, 
        .ctx_free_fn = NULL
    };

    esp_local_ctrl_add_property(&property);
}

esp_err_t LocalCtrlHandler::getPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx) {
    ConfigProviderIfc* pProvider = static_cast<ConfigProviderIfc*>(usr_ctx);

    for (uint32_t i = 0; i < props_count; i++) {
        // set default
        prop_values[i].size = (static_cast<PropertyType>(props[i].type)).getSize();
        prop_values[i].data = NULL;

        esp_err_t err = pProvider->readConfiguration(props[i].name, &prop_values[i].data, &(prop_values[i].size));
        if (err != ESP_OK) return err;
    }

    return ESP_OK;
}

esp_err_t LocalCtrlHandler::setPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], const esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx) {
    ConfigProviderIfc* pProvider = static_cast<ConfigProviderIfc*>(usr_ctx);

    for (uint32_t i = 0; i < props_count; i++) {
        if (props[i].flags & PropertyFlags::PROP_FLAG_READONLY) {
            return ESP_ERR_INVALID_ARG;
        }   

        const PropertyType type = static_cast<PropertyType::Value>(props[i].type);
        const size_t dataSize = type.getSize() == 0 ? prop_values[i].size : type.getSize();

        if (dataSize == 0) {
            ESP_LOGW("HumiDevice", "DataSize is %i. Received Cmd but nut data available", dataSize);
            return ESP_ERR_INVALID_ARG;
        }

        esp_err_t err = pProvider->writeConfiguration(props[i].name, prop_values[i].data, dataSize);
        if (err != ESP_OK) return err;
    }

    return ESP_OK;
}
  
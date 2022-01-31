#include "LocalCtrlHandler.h"

#include "mdns.h"
#include "esp_https_server.h"

#define ESP_CTRL_NUMBER_OF_PROPERTIES 3
#define ESP_CTRL_SERVICE_NAME "arqms-device"

#define ESP_CTRL_PROP_DEVICE_SN "Device_SerialNumber"
#define ESP_CTRL_PROP_WIFI_SSID "Wifi_SSID"
#define ESP_CTRL_PROP_WIFI_PASSPHRASE "Wifi_Passphrase"


LocalCtrlHandler::LocalCtrlHandler(DeviceStorage& storage) :
    storage(storage),
    serviceEnabled(false) {
    
}

void LocalCtrlHandler::startService(void) {
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
        .transport = ESP_LOCAL_CTRL_TRANSPORT_HTTPD,
        .transport_config = {
            .httpd = &httpsCfg
        },
        .proto_sec = {
            .version = PROTOCOM_SEC1,
            .custom_handle = NULL,
            .pop = NULL,
        },
        .handlers = {
            .get_prop_values = &LocalCtrlHandler::getPropertyValues,
            .set_prop_values = &LocalCtrlHandler::setPropertyValues,
            .usr_ctx         = &storage,
            .usr_ctx_free_fn = NULL
        },
        // Maximum number of properties that may be set
        .max_properties = ESP_CTRL_NUMBER_OF_PROPERTIES
    };

    mdns_init();
    mdns_hostname_set(ESP_CTRL_SERVICE_NAME);

    // register properties
    registerProperty(ESP_CTRL_PROP_DEVICE_SN, PropertyType::PROP_TYPE_UINT32, true);
    registerProperty(ESP_CTRL_PROP_WIFI_SSID, PropertyType::PROP_TYPE_CHAR_STRING);
    registerProperty(ESP_CTRL_PROP_WIFI_PASSPHRASE, PropertyType::PROP_TYPE_CHAR_STRING);

    // start service
    const esp_err_t err = esp_local_ctrl_start(&config);
    serviceEnabled = err == ERR_OK;
}

void LocalCtrlHandler::stopService() {
    const esp_err_t err = esp_local_ctrl_stop();
    serviceEnabled = !(err == ERR_OK);
}

bool LocalCtrlHandler::isRunning() const {
    return serviceEnabled;
}

void LocalCtrlHandler::registerProperty(const char* name, const PropertyType type, const bool isReadOnly) {
    PropertyFlags flags = isReadOnly ? PropertyFlags::PROP_FLAG_READONLY : PropertyFlags::NONE;

    const esp_local_ctrl_prop_t property = {
        .name        = const_cast<char*>(name),
        .type        = type,
        .size        = type.getSize(),
        .flags       = static_cast<uint32_t>(flags),
        // TODO pointer to data! Maybe a temp struct in this class. When service is stopped without errors, we save the configuration to Storage
        .ctx         = NULL, 
        .ctx_free_fn = NULL
    };

    esp_local_ctrl_add_property(&property);
}

esp_err_t LocalCtrlHandler::getPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx) {
    for (uint32_t i = 0; i < props_count; i++) {
        switch (props[i].type) {
            case PropertyType::PROP_TYPE_UINT8:
            case PropertyType::PROP_TYPE_UINT16:
            case PropertyType::PROP_TYPE_UINT32:
            case PropertyType::PROP_TYPE_UINT64:
            case PropertyType::PROP_TYPE_INT8:
            case PropertyType::PROP_TYPE_INT16:
            case PropertyType::PROP_TYPE_INT32:
            case PropertyType::PROP_TYPE_INT64:
            case PropertyType::PROP_TYPE_FLOAT32:
            case PropertyType::PROP_TYPE_BOOL: {
                prop_values[i].data = props[i].ctx;
            }

            case PropertyType::PROP_TYPE_CHAR_STRING: {
                // TODO
            }
            break;
        }
    }

    return ESP_OK;
}

esp_err_t LocalCtrlHandler::setPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], const esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx) {
    for (uint32_t i = 0; i < props_count; i++) {
        if (props[i].flags & PropertyFlags::PROP_FLAG_READONLY) {
            return ESP_ERR_INVALID_ARG;
        }   

        const PropertyType type = static_cast<PropertyType::Value>(props[i].type);
        switch (type) {
            case PropertyType::PROP_TYPE_UINT8:
            case PropertyType::PROP_TYPE_UINT16:
            case PropertyType::PROP_TYPE_UINT32:
            case PropertyType::PROP_TYPE_UINT64:
            case PropertyType::PROP_TYPE_INT8:
            case PropertyType::PROP_TYPE_INT16:
            case PropertyType::PROP_TYPE_INT32:
            case PropertyType::PROP_TYPE_INT64:
            case PropertyType::PROP_TYPE_FLOAT32:
            case PropertyType::PROP_TYPE_BOOL: {
                memcpy(props[i].ctx, prop_values[i].data, type.getSize());
            }
            break;

            case PropertyType::PROP_TYPE_CHAR_STRING: {
                if (!prop_values[i].size) return ESP_ERR_INVALID_SIZE;
    
                // TODO
            }
            break;
        }

    }

    return ESP_OK;
}
  
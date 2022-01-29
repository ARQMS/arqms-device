#include "WiFiManager.h"
#include "DeviceStorage/ConfigurationLayout.h"

#include "esp_wifi.h"


WiFiManager::WiFiManager(DeviceStorage& storage) :
    storage(storage) {
}

WiFiManager::~WiFiManager() {
}

void WiFiManager::initialize() {
    ESP_ERROR_CHECK(storage.isInitialized());

    // initialize WiFi Peripheral, no WIFI model startet yet
    wifi_init_config_t initCfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&initCfg);

    // initialize TCP stack
    esp_netif_init();
    esp_event_loop_create_default();
}

bool WiFiManager::isSTACfgValid() {
    WifiParameters wifiConfig = storage.getWifiParameters();
    
    // After NVS is erased, the SSID is empty. so we can be sure there
    // is no valid configuration.
    bool hasSSID = strlen((char*)wifiConfig.sta.ssid) != 0;
    return hasSSID;
}
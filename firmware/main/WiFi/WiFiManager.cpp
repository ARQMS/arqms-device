#include "WiFiManager.h"

#include "esp_wifi.h"


WiFiManager::WiFiManager() {
}

WiFiManager::~WiFiManager() {
}

void WiFiManager::initialize() {
    // initialize WiFi Peripheral, no WIFI model startet yet
    wifi_init_config_t initCfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&initCfg);

    // initialize TCP stack
    esp_netif_init();
    esp_event_loop_create_default();
}
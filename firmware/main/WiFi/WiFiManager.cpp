#include "WiFiManager.h"

#include "esp_wifi.h"


// Event Callback mapper
static void eventHandler(void* context, esp_event_base_t base, int32_t id, void* data) {
    EventHandlerIfc* pHandler = static_cast<EventHandlerIfc*>(context);
    pHandler->onEvent(base, id, data);
}

WiFiManager::WiFiManager(DeviceStorage& storage, EventLoopIfc& eventLoop) :
    storage(storage),
    eventLoop(eventLoop),
    wifiState(WiFiState::DISABLED),
    retryNum(0U),
    apClients(0U) {
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

    // Register events from default loop
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandler, this, NULL);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandler, this, NULL);
}

void WiFiManager::onEvent(esp_event_base_t base, int32_t id, void* data) {
    // STA
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
        if (retryNum < MAXIMUM_RECONNECTING_RETRY) {
            esp_wifi_connect();
            retryNum++;
        } else {
            wifiState = WiFiState::FAILED;
        }
    } 
    else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        retryNum = 0;
        wifiState = WiFiState::CONNECTED;
    }

    // AP
    else if (base == WIFI_EVENT && id == WIFI_EVENT_AP_STACONNECTED) {
        apClients++;
    }
    else if (base == WIFI_EVENT && id == WIFI_EVENT_AP_STADISCONNECTED) {
        apClients--;
    }
}

WiFiState WiFiManager::getWifiState() const {
    return wifiState;
}

uint32_t WiFiManager::countClients() {
    return apClients;
}

void WiFiManager::startSTA(WifiParameters& cfg) {
    esp_netif_create_default_wifi_sta();

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &cfg.config);
    esp_wifi_start();
}

void WiFiManager::startAP(WifiParameters& cfg) {
    esp_netif_create_default_wifi_ap();

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &cfg.config);
    esp_wifi_start();
}

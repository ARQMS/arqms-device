#include "Wifi.h"

// idf includes
#include "sdkconfig.h"

#define HUMIDEVICE_ESP_WIFI_SSID      CONFIG_SERVICE_WIFI_SSID
#define HUMIDEVICE_ESP_WIFI_PASS      CONFIG_SERVICE_WIFI_PASSWORD
#define HUMIDEVICE_ESP_WIFI_CHANNEL   CONFIG_SERVICE_WIFI_CHANNEL
#define HUMIDEVICE_MAX_STA_CONN       1

Wifi::Wifi(CloudLinkSenderIfc& wifiCallback, ConfigurationServiceIfc& ctrlHandler, MqttServiceIfc& mqttService) : 
    m_wifiSettings(),
    m_deviceSettings(),
    m_wifiSm(),
    m_sender(wifiCallback),
    m_retryCounter(0U),
    m_ctrlHandler(ctrlHandler),
    m_mqttService(mqttService),
    m_pEspNet(NULL) {
}

Wifi::~Wifi() {
}

void Wifi::initialize() {
    if (m_wifiSm.isCurrentState(WifiStateMachine::State::OFF)) {
        checkEspError(esp_netif_init());
        checkEspError(esp_event_loop_create_default());
    }
    else if (m_wifiSm.isCurrentState(WifiStateMachine::State::AP_SERVICE)
     || m_wifiSm.isCurrentState(WifiStateMachine::State::AP_SERVICE_WAITING)) {
        reset();
        startServiceMode();
    }
    else if (m_wifiSm.isCurrentState(WifiStateMachine::State::STA_CONNECTED)
     || m_wifiSm.isCurrentState(WifiStateMachine::State::STA_NORMAL_CONNECTING)) {
        reset();
        startNormalMode();
    }
}

void Wifi::startServiceMode() {
    m_wifiSm.onApMode();

    m_pEspNet = esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    checkEspError(esp_wifi_init(&cfg));
    checkEspError(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &Wifi::onWifiEventHandler, (void*)this, NULL));

    wifi_config_t wifiConfig = {};
    wifiConfig.ap.channel = HUMIDEVICE_ESP_WIFI_CHANNEL;
    wifiConfig.ap.ssid_len = sizeof(HUMIDEVICE_ESP_WIFI_SSID);
    wifiConfig.ap.max_connection = 1;
    wifiConfig.ap.authmode = WIFI_AUTH_WPA2_PSK;
    memcpy(&wifiConfig.ap.ssid, HUMIDEVICE_ESP_WIFI_SSID, sizeof(HUMIDEVICE_ESP_WIFI_SSID));
    memcpy(&wifiConfig.ap.password, HUMIDEVICE_ESP_WIFI_PASS, sizeof(HUMIDEVICE_ESP_WIFI_PASS));

    checkEspError(esp_wifi_set_mode(WIFI_MODE_AP));
    checkEspError(esp_wifi_set_config(WIFI_IF_AP, &wifiConfig));
    checkEspError(esp_wifi_start());
}

void Wifi::startNormalMode() {
    m_wifiSm.onStaMode();

    m_pEspNet = esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    checkEspError(esp_wifi_init(&cfg));
    checkEspError(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &Wifi::onWifiEventHandler, (void*)this, NULL));

    char8_t ssid[WifiSettingsEvent::MAX_SSID_LENGTH];
    char8_t pwd[WifiSettingsEvent::MAX_PWD_LENGTH];
    m_wifiSettings.getSsid(ssid);
    m_wifiSettings.getPassword(pwd);

    wifi_config_t wifiConfig = {};
    wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    memcpy(&wifiConfig.sta.ssid, ssid, m_wifiSettings.MAX_SSID_LENGTH);
    memcpy(&wifiConfig.sta.password, pwd, m_wifiSettings.MAX_PWD_LENGTH);
    
    checkEspError(esp_wifi_set_mode(WIFI_MODE_STA));
    checkEspError(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    checkEspError(esp_wifi_start());
}

void Wifi::updateWifiSettings(const WifiSettingsEvent& settings) {
    m_wifiSettings = settings;
    m_wifiSettingsUpdated = true;

    if (m_deviceSettingsUpdated && m_wifiSettingsUpdated) {
        initialize();
    }
}

void Wifi::updateDeviceSettings(const DeviceInfoEvent& settings) {
    m_deviceSettings = settings;
    m_deviceSettingsUpdated = true;

    if (m_deviceSettingsUpdated && m_wifiSettingsUpdated) {
        initialize();
    }
}

void Wifi::onTimeout() {
    if (m_wifiSm.isCurrentState(WifiStateMachine::State::AP_SERVICE_WAITING)
     || m_wifiSm.isCurrentState(WifiStateMachine::State::STA_NORMAL_CONNECTING)) {
        reset();
        m_sender.sendWifiStatus(WifiStatus::TIMEOUT);
    }
}

void Wifi::reset() {
    if (m_wifiSm.isCurrentState(WifiStateMachine::State::OFF)) {
        return;
    }

    ESP_LOGI("HumiDevice", "Reset wifi phy");

    m_wifiSm.reset();

    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &Wifi::onWifiEventHandler);

    if (m_pEspNet != NULL) {
        esp_netif_destroy(m_pEspNet);
    }
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_netif_deinit();
    esp_wifi_deinit();
}

int32_t Wifi::getSignalStrength() const {
    if (!m_wifiSm.isCurrentState(WifiStateMachine::State::STA_CONNECTED)) {
        return INVALID_RSSI;
    }

    wifi_ap_record_t ap;
    esp_wifi_sta_get_ap_info(&ap);

    return ap.rssi;
}

void Wifi::onClientConnected() {
    if (!m_wifiSm.isCurrentState(WifiStateMachine::State::AP_SERVICE_WAITING)) {
        // It's not allowd to connect to device when not in service waiting mode
        m_sender.sendWifiStatus(WifiStatus::UNKNOWN_ERROR);
        return;
    }

    checkEspError(m_ctrlHandler.startService());

    m_wifiSm.onClientConnected();
    m_sender.sendWifiStatus(WifiStatus::CLIENT_CONNECTED);
}

void Wifi::onClientDisconnected() {
    m_wifiSm.onClientDisconnected();

    // all clients disconnected
    if (m_wifiSm.isCurrentState(WifiStateMachine::State::OFF)) {
        m_sender.sendWifiStatus(WifiStatus::CLIENT_DISCONNECTED);
    }
}

void Wifi::onApStarted() {
    m_sender.sendWifiStatus(WifiStatus::CLIENT_SEARCHING);
}

void Wifi::onStaStarted() {
    m_sender.sendWifiStatus(WifiStatus::CONNECTING, getSignalStrength());
    esp_wifi_connect();
}

void Wifi::onWifiConnected() {
    checkEspError(m_mqttService.startService(m_deviceSettings));

    m_wifiSm.onGotIp();
    m_retryCounter = 0U;

    m_sender.sendWifiStatus(WifiStatus::CONNECTED, getSignalStrength());
}

void Wifi::onWifiDisconnected() {
    if (m_wifiSm.isCurrentState(WifiStateMachine::State::OFF)) { 
        return; 
    }

    m_wifiSm.onLostIp();

    if (m_retryCounter < MAXIMUM_RETRY) {
        m_sender.sendWifiStatus(WifiStatus::CONNECTING, getSignalStrength());
        esp_wifi_connect();
        m_retryCounter++;
    }
    else {
        m_sender.sendWifiStatus(WifiStatus::DISCONNECTED, getSignalStrength());
    }
}

void Wifi::onWifiEventHandler(void* param, esp_event_base_t eventBase, int32_t eventId, void* eventData) {
    Wifi* pSender = (Wifi*)(param);

    // there was an failure during initialization, skip this event and reset PHY first!
    if (pSender->m_wifiSm.isCurrentState(WifiStateMachine::State::FAILURE)) return;

    if (eventId == WIFI_EVENT_AP_STACONNECTED) {
        pSender->onClientConnected();
    }
    else if (eventId == WIFI_EVENT_AP_STADISCONNECTED) {
        pSender->onClientDisconnected();
    } 
    else if (eventId == WIFI_EVENT_AP_START) {
        pSender->onApStarted();
    }
    else if (eventId == WIFI_EVENT_STA_START) {
        pSender->onStaStarted();
    }
    else if (eventId == WIFI_EVENT_STA_CONNECTED) {
        pSender->onWifiConnected();
    }
    else if (eventId == WIFI_EVENT_STA_DISCONNECTED) {
        pSender->onWifiDisconnected();
    }
}

void Wifi::checkEspError(const esp_err_t status) {
    if (status != ESP_OK) {
        ESP_LOGE("HumiDevice", "Error in Wifi: %s", esp_err_to_name(status));
        m_wifiSm.onFailure();

        m_sender.sendWifiStatus(WifiStatus::UNKNOWN_ERROR);
    }
}

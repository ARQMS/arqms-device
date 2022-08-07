#include "WifiStateMachine.h"

// idf includes
#include "sdkconfig.h"

#define HUMIDEVICE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define HUMIDEVICE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define HUMIDEVICE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define HUMIDEVICE_MAX_STA_CONN       1

WifiStateMachine::WifiStateMachine(WifiStateMachineIfc& sender, LocalCtrlHandlerIfc& ctrlHandler) :
    m_sender(sender),
    m_ctrlHandler(ctrlHandler),
    m_wifiSettings(),
    m_currentState(State::OFF),
    m_nextState(State::OFF),
    m_clientConnected(false),
    m_clientDisonnected(false),
    m_serviceWaiting(false),
    m_connected(false),
    m_connectionLost(false),
    m_normalConnecting(false),
    m_failure(false),
    m_retryNum(0U) {
}

WifiStateMachine::~WifiStateMachine() {
}

void WifiStateMachine::reset() {   
    m_failure = false;

    m_connected = false;
    m_clientConnected = false;
    m_clientDisonnected = false;
    m_serviceWaiting = false;
    m_normalConnecting = false;
    m_connectionLost = false;

    m_retryNum = 0U;

    m_nextState = State::OFF;
    m_currentState = State::OFF;

    ESP_LOGI("HumiDevice", "Reset wifi phy");

    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &onWifiEventHandler);
    esp_wifi_stop();
    esp_netif_deinit();
    esp_wifi_deinit();
}

void WifiStateMachine::startServiceMode(const WifiSettingsEvent& wifiSetting) {
    m_wifiSettings = wifiSetting;

    m_serviceWaiting = true;
    runStateMachine();
}

void WifiStateMachine::startNormalMode(const WifiSettingsEvent& wifiSetting) {
    m_wifiSettings = wifiSetting;

    m_normalConnecting = true;
    runStateMachine();
}

void WifiStateMachine::onClientConnected() {
    m_clientConnected = true;
    runStateMachine();

    m_sender.sendWifiStatus(WifiStatus::CLIENT_CONNECTED);
}

void WifiStateMachine::onClientDisconnected() {
    m_clientDisonnected = true;
    runStateMachine();

    m_sender.sendWifiStatus(WifiStatus::CLIENT_DISCONNECTED);
}

void WifiStateMachine::onClientSearching() {
    runStateMachine();
    m_sender.sendWifiStatus(WifiStatus::CLIENT_SEARCHING);
}

void WifiStateMachine::onWifiConnecting() {
    m_sender.sendWifiStatus(WifiStatus::CONNECTING);

    esp_wifi_connect();
    runStateMachine();
}

void WifiStateMachine::onWifiConnected() {
    m_connected = true;
    m_retryNum = 0;
    runStateMachine();

    wifi_ap_record_t ap;
    esp_wifi_sta_get_ap_info(&ap);
    m_sender.sendWifiStatus(WifiStatus::CONNECTED, ap.rssi);
}

void WifiStateMachine::onWifiDisconnected() {
    if (m_retryNum < MAXIMUM_RETRY) {
        esp_wifi_connect();
        m_retryNum++;
    }
    else {
        m_connectionLost = true;
    }

    runStateMachine();
    m_sender.sendWifiStatus(WifiStatus::DISCONNECTED, -100);
}

bool WifiStateMachine::isCurrentState(const State state) const {
    return m_currentState == state;
}

void WifiStateMachine::runStateMachine(void) {
    bool stateChanged = true;

    do {
        stateChanged = false;
        switch (m_currentState) {
            case State::OFF:
                handleEvent(&m_serviceWaiting, State::SERVICE_WAITING);
                handleEvent(&m_normalConnecting, State::NORMAL_CONNECTING);
                break;

            case State::SERVICE_WAITING:
                handleEvent(&m_clientConnected, State::SERVICE);
                break;

            case State::NORMAL_CONNECTING:
                handleEvent(&m_connected, State::NORMAL);
                break;

            case State::NORMAL:
                handleEvent(&m_connectionLost, State::NORMAL_CONNECTING);
                break;

            case State::SERVICE:
                handleEvent(&m_clientDisonnected, State::OFF);
                break;

            case State::FAILURE:
                // nothing to do. reset()
                break;

            default:
                ESP_LOGE("HumiDevice", "unexpected WifiStateMachine::State");
                break;
        }

        // any state can throw a failure
        handleEvent(&m_failure, State::FAILURE);

        if (m_currentState != m_nextState) {
            onLeaveState(m_currentState);
            stateChanged = true;
            m_currentState = m_nextState;
            onEnterState(m_currentState);
        }

        // Run current state
        onRunState(m_currentState);

    } while (stateChanged);
}

void WifiStateMachine::onEnterState(const State state) { 
    switch (m_currentState) {
        case State::SERVICE_WAITING:          
            startWifiAsAp();
            break;

        case State::NORMAL_CONNECTING:
            startWifiAsSta();
            break;

        case State::FAILURE:
            reset();
            break;

        default: 
            // nothnig to do
            break;
    }
}

void WifiStateMachine::onRunState(const State state) const { 
    // do nothing
}

void WifiStateMachine::onLeaveState(const State state) { 
    switch (m_currentState) {
        case State::OFF: 
            checkEspError(esp_netif_init());
            checkEspError(esp_event_loop_create_default());
            break;

        case State::SERVICE:
            checkEspError(m_ctrlHandler.stopService());
            break;

        default: 
            // nothnig to do
            break;
    }
}

void WifiStateMachine::onWifiEventHandler(void* param, esp_event_base_t eventBase, int32_t eventId, void* eventData) {
    WifiStateMachine* pSender = (WifiStateMachine*)(param);

    // there was an failure during initialization, skip this event and reset PHY first!
    if (pSender->m_failure || pSender->isCurrentState(WifiStateMachine::State::FAILURE)) return;

    if (eventId == WIFI_EVENT_AP_STACONNECTED) {
        pSender->onClientConnected();
    }
    else if (eventId == WIFI_EVENT_AP_STADISCONNECTED) {
        pSender->onClientDisconnected();
    } 
    else if (eventId == WIFI_EVENT_AP_START) {
        pSender->onClientSearching();
    }
    else if (eventId == WIFI_EVENT_STA_START) {
        pSender->onWifiConnecting();
    }
    else if (eventId == WIFI_EVENT_STA_CONNECTED) {
        pSender->onWifiConnected();
    }
    else if (eventId == WIFI_EVENT_STA_DISCONNECTED) {
        pSender->onWifiDisconnected();
    }
}

void WifiStateMachine::startWifiAsAp() {
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    checkEspError(esp_wifi_init(&cfg));
    checkEspError(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiStateMachine::onWifiEventHandler, (void*)this, NULL));

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

    checkEspError(m_ctrlHandler.startService());
}

void WifiStateMachine::startWifiAsSta() {
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    checkEspError(esp_wifi_init(&cfg));
    checkEspError(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WifiStateMachine::onWifiEventHandler, (void*)this, NULL));

    char8_t ssid[WifiSettingsEvent::MAX_SSID_LENGTH];
    char8_t pwd[WifiSettingsEvent::MAX_PWD_LENGTH];
    m_wifiSettings.getSSID(ssid);
    m_wifiSettings.getPWD(pwd);

    wifi_config_t wifiConfig = {};
    wifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    memcpy(&wifiConfig.sta.ssid, ssid, m_wifiSettings.MAX_SSID_LENGTH);
    memcpy(&wifiConfig.sta.password, pwd, m_wifiSettings.MAX_PWD_LENGTH);

    checkEspError(esp_wifi_set_mode(WIFI_MODE_STA));
    checkEspError(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
    checkEspError(esp_wifi_start());
}

void WifiStateMachine::handleEvent(bool* const pFlag, const State nextState) { 
    if (pFlag == NULL) {
        return;
    }
    if (!(*pFlag)) {
        return;
    }

    m_nextState = nextState;

    *pFlag = false;
}

void WifiStateMachine::checkEspError(const esp_err_t status) {
    if (status != ESP_OK) {
        ESP_LOGE("HumiDevice", "Error in WifiStateMachine: %s", esp_err_to_name(status));
        m_failure = true;

        m_sender.sendWifiStatus(WifiStatus::UNKNOWN_ERROR);
    }
}

#include "WifiStateMachine.h"

// idf includes
#include "esp_wifi.h"
#include "sdkconfig.h"


#define HUMIDEVICE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define HUMIDEVICE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define HUMIDEVICE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define HUMIDEVICE_MAX_STA_CONN       1

extern "C" void onWifiEventHandler(void* param, esp_event_base_t eventBase, int32_t eventId, void* eventData) {
    WifiStateMachine* pSender = (WifiStateMachine*)(param);

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

WifiStateMachine::WifiStateMachine(WifiStateMachineIfc& sender, LocalCtrlHandlerIfc& ctrlHandler) :
    m_sender(sender),
    m_ctrlHandler(ctrlHandler),
    m_currentState(State::OFF),
    m_nextState(State::OFF),
    m_clientConnected(false),
    m_clientDisonnected(false),
    m_serviceWaiting(false),
    m_normalMode(false),
    m_failure(false) {
}

WifiStateMachine::~WifiStateMachine() {
}

void WifiStateMachine::reset() {   
    m_failure = false;

    m_normalMode = false;
    m_clientConnected = false;
    m_clientDisonnected = false;
    m_serviceWaiting = false;

    m_nextState = State::OFF;
    m_currentState = State::OFF;

    ESP_LOGI("HumiDevice", "Reset wifi phy");

    esp_netif_deinit();
    esp_wifi_deinit();
}

void WifiStateMachine::onStartServiceMode() {
    m_serviceWaiting = true;
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
    runStateMachine();
    m_sender.sendWifiStatus(WifiStatus::CONNECTING);
}

void WifiStateMachine::onWifiConnected() {
    runStateMachine();

    wifi_ap_record_t ap;
    esp_wifi_sta_get_ap_info(&ap);
    m_sender.sendWifiStatus(WifiStatus::CONNECTED, ap.rssi);
}

void WifiStateMachine::onWifiDisconnected() {
    runStateMachine();
    // -100dB is bad rssi
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
                handleEvent(&m_normalMode, State::NORMAL);
                break;

            case State::SERVICE_WAITING:
                handleEvent(&m_clientConnected, State::SERVICE);
                break;

            case State::NORMAL:
                break;

            case State::SERVICE:
                handleEvent(&m_clientDisonnected, State::OFF);
                break;

            case State::FAILURE: 
                // nothing to do
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

        case State::SERVICE_WAITING: 
            checkEspError(m_ctrlHandler.startService());
            break;

        case State::SERVICE:
            checkEspError(m_ctrlHandler.stopService());
            break;

        default: 
            // nothnig to do
            break;
    }
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

void WifiStateMachine::startWifiAsAp() {
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    checkEspError(esp_wifi_init(&cfg));
    checkEspError(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &onWifiEventHandler, (void*)this, NULL));
    
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

void WifiStateMachine::checkEspError(const esp_err_t status) {
    if (status != ESP_OK) {
        ESP_LOGE("HumiDevice", "WifiStateMachine has unknown failure. Reset PHY");

        m_sender.sendWifiStatus(WifiStatus::UNKNOWN_ERROR);

        m_failure = true;
    }
}

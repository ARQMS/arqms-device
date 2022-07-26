#include "WifiStateMachine.h"

// idf includes
#include "esp_wifi.h"
#include "sdkconfig.h"

#define HUMIDEVICE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define HUMIDEVICE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define HUMIDEVICE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define HUMIDEVICE_MAX_STA_CONN       1

extern "C" void onWifiEventHandler(void* param, esp_event_base_t eventBase, int32_t eventId, void* eventData) {
    WifiStateMachineIfc* pSender = (WifiStateMachineIfc*)(param);

    if (eventId == WIFI_EVENT_AP_STACONNECTED) { 
        pSender->onClientConnected();
    }
    else if (eventId == WIFI_EVENT_AP_STADISCONNECTED) {
        pSender->onClientDisconnected();
    } 
    else if (eventId == WIFI_EVENT_AP_START) {
        pSender->onServiceModeIdle();
    }
    else if (eventId == WIFI_EVENT_STA_START) {
        pSender->onNormalModeIdle();
    }
    else if (eventId == WIFI_EVENT_STA_CONNECTED) {
        // TODO wifi connected
    }
    else if (eventId == WIFI_EVENT_STA_DISCONNECTED) {
        // TODO wifi disconnected
    }
}

WifiStateMachine::WifiStateMachine(WifiStateMachineIfc& sender) :
    m_sender(sender),
    m_currentState(State::OFF),
    m_nextState(State::OFF),
    m_serviceMode(false),
    m_normalMode(false),
    m_failure(false) {
}

WifiStateMachine::~WifiStateMachine() {
}

void WifiStateMachine::reset() {
    m_failure = false;
    m_normalMode = false;
    m_serviceMode = false;

    m_currentState = State::OFF;
    m_nextState = State::OFF;

    ESP_LOGI("HumiDevice", "Reset wifi phy");

    esp_netif_deinit();
    esp_wifi_deinit();

    runStateMachine();
}

void WifiStateMachine::onServiceMode() {
    m_serviceMode = true;
    runStateMachine();
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
                handleEvent(&m_serviceMode, State::SERVICE);
                handleEvent(&m_normalMode, State::NORMAL);
                break;

            case State::NORMAL:
                break;

            case State::SERVICE:
                break;

            case State::FAILURE: 
                reset();
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
        case State::SERVICE:          
            startWifiAsAp();
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
        case State::OFF: {
            m_failure |= esp_netif_init() != ESP_OK;
            m_failure |= esp_event_loop_create_default() != ESP_OK;
        }
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
    m_failure |= esp_wifi_init(&cfg) != ESP_OK;
    m_failure |= esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &onWifiEventHandler, (void*)&m_sender, NULL) != ESP_OK;
    
    wifi_config_t wifiConfig = {};
    wifiConfig.ap.channel = HUMIDEVICE_ESP_WIFI_CHANNEL;
    wifiConfig.ap.ssid_len = sizeof(HUMIDEVICE_ESP_WIFI_SSID);
    wifiConfig.ap.max_connection = 1;
    wifiConfig.ap.authmode = WIFI_AUTH_WPA2_PSK;
    memcpy(&wifiConfig.ap.ssid, HUMIDEVICE_ESP_WIFI_SSID, sizeof(HUMIDEVICE_ESP_WIFI_SSID));
    memcpy(&wifiConfig.ap.password, HUMIDEVICE_ESP_WIFI_PASS, sizeof(HUMIDEVICE_ESP_WIFI_PASS));

    m_failure |= esp_wifi_set_mode(WIFI_MODE_AP) != ESP_OK;
    m_failure |= esp_wifi_set_config(WIFI_IF_AP, &wifiConfig) != ESP_OK;
    m_failure |= esp_wifi_start() != ESP_OK;
}

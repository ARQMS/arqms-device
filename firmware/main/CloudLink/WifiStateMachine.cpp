#include "WifiStateMachine.h"

// idf includes
#include "esp_wifi.h"
#include "sdkconfig.h"

#define HUMIDEVICE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define HUMIDEVICE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define HUMIDEVICE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define HUMIDEVICE_MAX_STA_CONN       1

extern "C" void onWifiEventHandler(void* param, esp_event_base_t eventBase, int32_t eventId, void* eventData) {
    WifiStateMachine* pStateMachine = (WifiStateMachine*)(param);

    if (eventId == WIFI_EVENT_AP_STACONNECTED) {
        // client connected
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) eventData;
        ESP_LOGI("HumidDevice", "station "MACSTR" join, AID=%d", MAC2STR(event->mac), event->aid);

    }
    else if (eventId == WIFI_EVENT_AP_STADISCONNECTED) {
        // client disconnected
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) eventData;
        ESP_LOGI("HumidDevice", "station "MACSTR" leave, AID=%d", MAC2STR(event->mac), event->aid);
    }
}

WifiStateMachine::WifiStateMachine() :
    m_currentState(State::OFF),
    m_nextState(State::OFF),
    m_initialized(false),
    m_serviceMode(false),
    m_normalMode(false),
    m_failure(false) {
}

WifiStateMachine::~WifiStateMachine() {
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
                handleEvent(&m_failure, State::FAILURE);
                break;

            case State::SERVICE:
                handleEvent(&m_failure, State::FAILURE);

                break;

            case State::FAILURE: // TODO TBD
            default:
                ESP_LOGE("HumiDevice", "unexpected WifiStateMachine::State");
                break;
        }

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
        case State::SERVICE: {
            esp_netif_create_default_wifi_ap();
            wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
            m_failure |= esp_wifi_init(&cfg) != ESP_OK;
            m_failure |= esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &onWifiEventHandler, (void*)this, NULL) != ESP_OK;
           //
           //wifi_config_t wifiConfig;
           //wifiConfig.ap.ssid = HUMIDEVICE_ESP_WIFI_SSID;

           //m_failure |= esp_wifi_set_mode(WIFI_MODE_AP) != ESP_OK;
           //m_failure |= esp_wifi_set_config(WIFI_IF_AP, &wifiConfig) != ESP_OK;
           //m_failure |= esp_wifi_start() != ESP_OK;
        }
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


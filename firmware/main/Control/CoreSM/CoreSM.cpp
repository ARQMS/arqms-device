#include "CoreSM.h"

#include "Events/WifiSettingsEvent.h"

CoreSM::CoreSM(CoreSMIfc& sender) :
    m_sender(sender),
    m_currentState(State::BootUp),
    m_nextState(State::BootUp),
    m_isService(false),
    m_isConnecting(false),
    m_isConnected(false),
    m_isConnectionLost(false),
    m_isBooting(false),
    m_activeWifiSettings() {
}

CoreSM::~CoreSM() {
}

void CoreSM::onServiceMode() {
    m_isService = true;
    m_activeWifiSettings.setPWD(NULL);
    m_activeWifiSettings.setSSID(NULL);
    m_activeWifiSettings.setMode(WifiMode::AP);

    runStateMachine();
}

void CoreSM::onNormalMode(const WifiParameters& param) {
    m_isConnecting = true;
    m_activeWifiSettings.setMode(WifiMode::STA);
    m_activeWifiSettings.setSSID(param.ssid);
    m_activeWifiSettings.setPWD(param.password);

    runStateMachine();
}

void CoreSM::onConnected() {
    m_isConnected = true;
    runStateMachine();
}

bool CoreSM::isCurrentState(const State state) const {
    return m_currentState == state;
}

void CoreSM::runStateMachine(void) {
    bool stateChanged = true;

    do {
        stateChanged = false;
        switch (m_currentState) {
            case State::BootUp:
                handleEvent(&m_isConnecting, State::Connecting);
                handleEvent(&m_isService, State::Service);
                break;
            
            case State::Service:
                break;

            case State::Connecting:
                handleEvent(&m_isConnected, State::Idle);
                break;

            case State::Idle:
                handleEvent(&m_isConnectionLost, State::Connecting);

                break;

            default:
                ESP_LOGE("HumiDevice", "unexpected CoreSM::State");
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

void CoreSM::onEnterState(const State state) { 
    switch (m_currentState) {
        case State::Service:
            m_sender.sendWifi(m_activeWifiSettings);
            break;

        case State::Connecting:
            m_sender.sendWifi(m_activeWifiSettings);
            break;

        case State::Idle:
            // TODO start sensor reading
            // m_sender.requestSensorData();
            ESP_LOGE("HumiDevice", "Not implemented yet!");

            break;

        default: 
            // nothnig to do
            break;
    }
}

void CoreSM::onRunState(const State state) const { 
    // do nothing
}

void CoreSM::onLeaveState(const State state) { 
    switch (m_currentState) {
        default: 
            // nothnig to do
            break;
    }
}

void CoreSM::handleEvent(bool* const pFlag, const State nextState) { 
    if (pFlag == NULL) {
        return;
    }
    if (!(*pFlag)) {
        return;
    }

    m_nextState = nextState;

    *pFlag = false;
}

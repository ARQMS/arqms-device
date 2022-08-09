#include "WifiStateMachine.h"

WifiStateMachine::WifiStateMachine() :
    m_currentState(State::OFF),
    m_nextState(State::OFF),
    m_failure(false), 
    m_startSta(false), 
    m_gotIp(false), 
    m_lostIp(false), 
    m_startAp (false),
    m_clientCounter(0U) {
}

WifiStateMachine::~WifiStateMachine() {
}

void WifiStateMachine::reset() {   
    m_failure = false;
    m_startSta = false;
    m_gotIp = false;
    m_lostIp = false;
    m_startAp = false;

    m_clientCounter = 0;

    m_nextState = State::OFF;
    m_currentState = State::OFF;
}

void WifiStateMachine::onStaMode() {
    reset();
    
    m_startSta = true;
    runStateMachine();
}

void WifiStateMachine::onGotIp() {
    m_gotIp = true;
    runStateMachine();
}

void WifiStateMachine::onLostIp() {
    m_lostIp = true;
    runStateMachine();
}

void WifiStateMachine::onApMode() {
    reset();

    m_startAp = true;
    runStateMachine();
}

void WifiStateMachine::onClientConnected() {
    m_clientCounter++;
    runStateMachine();
}

void WifiStateMachine::onClientDisconnected() {
    if (m_clientCounter > 0) {
        m_clientCounter--;
    }

    runStateMachine();
}

void WifiStateMachine::onFailure() {
    m_failure = true;
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
                handleEvent(&m_startAp, State::AP_SERVICE_WAITING);
                handleEvent(&m_startSta, State::STA_NORMAL_CONNECTING);
                break;

            case State::AP_SERVICE_WAITING:
                handleEvent(m_clientCounter > 0, State::AP_SERVICE);
                break;

            case State::STA_NORMAL_CONNECTING:
                handleEvent(&m_gotIp, State::STA_CONNECTED);
                break;

            case State::STA_CONNECTED:
                handleEvent(&m_lostIp, State::STA_NORMAL_CONNECTING);
                break;

            case State::AP_SERVICE:
                handleEvent(m_clientCounter <= 0, State::OFF);
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
    // nothnig to do
}

void WifiStateMachine::onRunState(const State state) const { 
    // do nothing
}

void WifiStateMachine::onLeaveState(const State state) { 
    // nothnig to do
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

void WifiStateMachine::handleEvent(const bool condition, const State nextState) { 
    if (!condition) {
        return;
    }

    m_nextState = nextState;
}

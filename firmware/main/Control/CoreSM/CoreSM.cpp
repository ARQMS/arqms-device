#include "CoreSM.h"

CoreSM::CoreSM() :
    m_currentState(State::BootUp),
    m_nextState(State::BootUp),
    m_isService(false),
    m_isRunning(false),
    m_isBooting(false) {
}

CoreSM::~CoreSM() {
}

void CoreSM::onServiceMode() {
    m_isService = true;
    runStateMachine();
}

void CoreSM::onRunning() {
    m_isRunning = true;
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
                handleEvent(&m_isRunning, State::Running);
                handleEvent(&m_isService, State::Service);
                break;
            
            case State::Service:
                // junction to running requires restart
                break;

            case State::Running:
                handleEvent(&m_isService, State::Service);
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
    // do nothing
}

void CoreSM::onRunState(const State state) const { 
    // do nothing
}

void CoreSM::onLeaveState(const State state) { 
    // nothnig to do
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

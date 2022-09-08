#include "ButtonSM.h"

ButtonSM::ButtonSM(const ButtonId id, ButtonSMIfc& sender) : 
    m_sender(sender),
    m_id(id),
    m_pTimerLongPress(NULL),
    m_currentState(State::RELEASED),
    m_nextState(State::RELEASED),
    m_isLongPressed(false),
    m_stateChanged(false) {
};

ButtonSM::~ButtonSM() {
};

void ButtonSM::initialize(TimerServiceIfc& timerService) {
    m_pTimerLongPress = timerService.createOneShotTimer(5000);
}

void ButtonSM::onHandleTimer(TimerId timerId) {
    m_isLongPressed = m_pTimerLongPress->id == timerId;

    runStateMachine();
}

void ButtonSM::onStateChanged() {
    m_stateChanged = true;
    runStateMachine();
}

void ButtonSM::runStateMachine() {
    bool stateChanged = true;

    do {
        stateChanged = false;
        switch (m_currentState) {
            case State::RELEASED:
                handleEvent(&m_stateChanged, State::WAITING);
                break;
            
            case State::WAITING:
                handleEvent(&m_stateChanged, State::SHORT_PRESSED);
                handleEvent(&m_isLongPressed, State::LONG_PRESSED);
                break;

            case State::SHORT_PRESSED: // fall through
            case State::LONG_PRESSED:
                m_nextState = State::RELEASED;
                break;

            default:
                ESP_LOGE("ButtonSM", "unexpected ButtonSM::State");
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

void ButtonSM::onEnterState(const State state) {
    switch (state) {
        case State::WAITING:
            m_pTimerLongPress->start();
            break;

        case State::SHORT_PRESSED:
            m_sender.onButtonPressed(m_id, ButtonStatus::SHORT_PRESS);
            break;

        case State::LONG_PRESSED:
            m_sender.onButtonPressed(m_id, ButtonStatus::LONG_PRESS);
            break;

        default:
            break;
    }
}

void ButtonSM::onRunState(const State state) const {
    // nothing to do
}

void ButtonSM::onLeaveState(const State state) {
    switch (state) {
        case State::WAITING:
            m_pTimerLongPress->stop();
            break;
            
        default:
            break;
    }
}

void ButtonSM::handleEvent(bool* const pFlag, const State nextState) {
    if (pFlag == NULL) {
        return;
    }
    if (!(*pFlag)) {
        return;
    }

    m_nextState = nextState;

    *pFlag = false;
}

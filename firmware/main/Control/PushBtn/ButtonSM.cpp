#include "ButtonSM.h"

ButtonSM::ButtonSM(const ButtonId id, ButtonSMIfc& sender) : 
    m_sender(sender),
    m_id(id),
    m_pressTick(0U),
    m_currentState(State::RELEASED),
    m_nextState(State::RELEASED),
    m_isShortPressed(false),
    m_isLongPressed(false),
    m_stateChangePending(false) {
}

ButtonSM::~ButtonSM() {
}

void ButtonSM::onStateChanged() {
    m_stateChangePending = true;

    if (m_currentState == State::WAITING) {
        m_isLongPressed = (xTaskGetTickCount() - m_pressTick) >= pdMS_TO_TICKS(LONG_PRESS_DURATION_MS);
        m_isShortPressed = !m_isLongPressed && m_stateChangePending;
    }

    runStateMachine();
}

void ButtonSM::runStateMachine() {
    bool stateChanged = true;

    do {
        stateChanged = false;
        switch (m_currentState) {
            case State::RELEASED:
                handleEvent(&m_stateChangePending, State::WAITING);
                break;
            
            case State::WAITING:
                handleEvent(&m_isShortPressed, State::SHORT_PRESSED);
                handleEvent(&m_isLongPressed, State::LONG_PRESSED);
                break;

            case State::SHORT_PRESSED:
                handleEvent(&m_stateChangePending, State::RELEASED);
                break;

            case State::LONG_PRESSED:
                handleEvent(&m_stateChangePending, State::RELEASED);
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

    } while (stateChanged);
}

void ButtonSM::onEnterState(const State state) {
    switch (state) {
        case State::WAITING:
            m_pressTick = xTaskGetTickCount();
            break;

        default:
            break;
    }
}

void ButtonSM::onLeaveState(const State state) {
    switch (state) {            
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

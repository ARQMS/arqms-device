#include "DeviceStateMachine.h"

#include "esp_log.h"

DeviceStateMachine::DeviceStateMachine() :
    currentState(State::INITIAL),
    nextState(State::INITIAL) {
}

DeviceStateMachine::~DeviceStateMachine() {
}

void DeviceStateMachine::process() {
    bool stateChanged = true;

    do {
        stateChanged = false;

        // switch case to check state conditionals
        switch (currentState) {
        case INITIAL: 
            break;
        case BOOT: 
            break;
        case SERVICE: 
            break;
        case WIFE_CONNECT: 
            break;
        case WIFI_CONFIG_HOTSPOT: 
            break;
        case RUNNING: 
            break;
        case SENDING_DATA: 
            break;
        case SENSOR_READ: 
            break;
        case RECEIVE_UPDATE: 
            break;
        default:
            ESP_LOGE("StateMachine", "State not implemented!");
            reset();
            break;
        }

        if (currentState != nextState) {
            onLeaveState(currentState);

            stateChanged = true;
            currentState = nextState;
            onEnterState(currentState);
        }

        onRunState(currentState);
    } while (stateChanged);
}

void DeviceStateMachine::reset() {
    currentState = State::INITIAL;
    nextState = State::INITIAL;
}

void DeviceStateMachine::onEnterState(const State state) {
    ESP_LOGV("StateMachine", "State %i entering", state);

    switch (state)     {
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::onRunState(const State state) {
    switch (state)     {
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::onLeaveState(const State state) {
    ESP_LOGV("StateMachine", "State %i leaving", state);

    switch (state)     {
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::conditionalStep(const bool isValid, const State state) {
    if (isValid) {
        nextState = state;
    }
}
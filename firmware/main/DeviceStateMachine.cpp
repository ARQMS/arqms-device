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

        switch (currentState) {
        case INITIAL: 
            break;
        case IDLE: 
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
            ESP_LOGE("State not implemented!");
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

}

void DeviceStateMachine::onEnterState(const State state) {
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
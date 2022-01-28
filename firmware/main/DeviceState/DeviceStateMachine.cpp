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
            // always valid to go to boot
            conditionalStep(true, State::BOOT);
            break;
        case BOOT: 
            // TODO wifi valid ? WIFE_CONNECT : WIFI_CONFIG_HOTSPOT
            break;
        case SERVICE: 
            // TODO !AnyClients ? BOOT
            break;
        case WIFI_CONNECTING: 
            // TODO WiFiConnected ? RUNNING
            // TIMEOUT or Failed ? WIFI_CONFIG_HOTSPOT
            break;
        case WIFI_CONFIG_HOTSPOT: 
            // TODO AnyClients : Service
            // TODO TIMEOUT SLEEP
            break;
        case RUNNING: 
            // TODO all Task at done and TIMEOUT ? SLEEP
            // TODO !WiFiConnected ? WIFI_CONNECTING
            // TODO NewVersionAvailable ? UPDATING
            break;
        case UPDATING:
            // TODO Update failed ? RUNNING : restart ESP
            break;
        case SLEEP:
            // nothing to do 
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
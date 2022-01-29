#include "DeviceStateMachine.h"

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

DeviceStateMachine::DeviceStateMachine(DeviceHandler& deviceHandler) :
    currentState(State::INITIAL),
    nextState(State::INITIAL),
    timeoutExpectedEnd(),
    handler(deviceHandler) {
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
            branchStep(handler.isWifiCfgValid(), CONNECTING, SERVICE_INITIALIZE);
            break;
        case SERVICE_INITIALIZE: 
            conditionalStep(timeoutReached(), SLEEP);
            conditionalStep(handler.anyClientConnected(), SERVICE);
            break;
        case SERVICE: 
            conditionalStep(!handler.anyClientConnected(), BOOT);
            break;
        case CONNECTING: 
            conditionalStep(handler.getWifiState() == WiFiState::FAILED, SERVICE_INITIALIZE);
            conditionalStep(handler.getWifiState() == WiFiState::CONNECTED, RUNNING);
            break;
        case RUNNING: 
            conditionalStep(handler.isAllTaskDone(), SLEEP);
            conditionalStep(handler.isNewVersionAvailable(), UPDATING);
            conditionalStep(handler.getWifiState() != WiFiState::CONNECTED, CONNECTING);
            break;
        case UPDATING:
            conditionalStep(handler.isFirmwareUpdateFailed(), RUNNING);
            break;
        case SLEEP:
            // nothing to do 
            break;
        default:
            ESP_LOGE("StateMachine", "State not implemented!");
            ESP_ERROR_CHECK(ESP_FAIL);
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

void DeviceStateMachine::initialize() {
    currentState = State::INITIAL;
    nextState = State::INITIAL;

    timeoutReset();
}

void DeviceStateMachine::onEnterState(const State state) const {
    ESP_LOGV("StateMachine", "State %i entering", state);

    switch (state) {
        case SERVICE_INITIALIZE:
            handler.enableAPMode();
            timeoutStart(3 * 60); // wait for 3min
            break;
        case CONNECTING:
            handler.connectToWiFi();
            break;
        case UPDATING:
            handler.startFirmwareUpdate();
            break;
        case RUNNING:
            handler.startDataAcquisition();
            break;
        case SLEEP:
            handler.enterSleep();
            break;
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::onRunState(const State state) const {
    switch (state) {
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::onLeaveState(const State state) const {
    ESP_LOGV("StateMachine", "State %i leaving", state);

    switch (state) {
    default:
        // nothing to do
        break;
    }

    // ensure timeout is stopped before state changes
    timeoutReset();
}

void DeviceStateMachine::conditionalStep(const bool conditional, const State valid) {
    if (conditional) {
        nextState = valid;
    }
}

void DeviceStateMachine::branchStep(const bool conditional, const State valid, const State invalid) {
    nextState = conditional ? valid : invalid;
}


bool DeviceStateMachine::timeoutReached() const {
    uint32_t current = static_cast<uint32_t>(xTaskGetTickCount());
    return current >= timeoutExpectedEnd;
}

void DeviceStateMachine::timeoutReset() const {
    timeoutExpectedEnd = portMAX_DELAY;
}

void DeviceStateMachine::timeoutStart(uint32_t durationS) const {
    timeoutExpectedEnd = static_cast<uint32_t>(xTaskGetTickCount()) + pdMS_TO_TICKS(durationS * 1000);
}
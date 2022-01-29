#include "DeviceStateMachine.h"
#include "freertos/FreeRTOS.h"

#include "esp_log.h"
#include "esp_event.h"

DeviceStateMachine::DeviceStateMachine(DeviceHandler& deviceHandler) :
    currentState(State::INITIAL),
    nextState(State::INITIAL),
    handler(deviceHandler) {
}

DeviceStateMachine::~DeviceStateMachine() {
}

void DeviceStateMachine::start(esp_event_loop_handle_t handle) {
    // Note: do not call process() here, this will start the state machine on current
    // task (which is main task!). The state machine should be called on his own task.
    // portMAX_DELAY just ensure the event is consumed, even the loop queue is alreay full (which 
    // should never happen at startup)
    esp_event_post_to(handle, STATEMACHINE_EVENTS, ESP_EVENT_ANY_ID, NULL, 0, portMAX_DELAY);
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
            // TODO AnyClients : Service
            // TODO TIMEOUT SLEEP
            break;
        case SERVICE: 
            // TODO !AnyClients ? BOOT
            break;
        case CONNECTING: 
            conditionalStep(handler.getWifiState() == WIFI_FAILED, SERVICE_INITIALIZE);
            conditionalStep(handler.getWifiState() == WIFI_CONNECTED, RUNNING);
            break;
        case RUNNING: 
            conditionalStep(handler.isAllTaskDone(), SLEEP);
            conditionalStep(handler.isNewVersionAvailable(), UPDATING);
            conditionalStep(handler.getWifiState() != WIFI_CONNECTED, CONNECTING);
            break;
        case UPDATING:
            // TODO Update failed ? RUNNING : restart ESP
            break;
        case SLEEP:
            // nothing to do 
            break;
        default:
            ESP_LOGE("StateMachine", "State not implemented!");
            initialize();
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
}

void DeviceStateMachine::onEnterState(const State state) {
    ESP_LOGV("StateMachine", "State %i entering", state);

    switch (state) {
        case SERVICE_INITIALIZE:
            // TODO handler.enableAPMode();
            // TODO startTimeout(5*60); // 5 min
            break;
        case CONNECTING:
            handler.connectToWiFi();
            startTimeout(1000 * 30); // 30 sec
            break;
        case UPDATING:
            // TODO handler.startFirmwareUpdate();
            break;
        case SLEEP:
            // TODO handler.enterSleep();
            break;
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::onRunState(const State state) {
    switch (state) {
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::onLeaveState(const State state) {
    ESP_LOGV("StateMachine", "State %i leaving", state);

    switch (state) {
    default:
        // nothing to do
        break;
    }
}

void DeviceStateMachine::startTimeout(const uint32_t durationMs) {
    // TODO Start new SW timer. After durationMs is passed, a new STATEMACHINE_EVENTS with TIMOUT_REACHED and 
    // currentState as argument should be sent
}

void DeviceStateMachine::conditionalStep(const bool conditional, const State valid) {
    if (conditional) {
        nextState = valid;
    }
}

void DeviceStateMachine::branchStep(const bool conditional, const State valid, const State invalid) {
    nextState = conditional ? valid : invalid;
}
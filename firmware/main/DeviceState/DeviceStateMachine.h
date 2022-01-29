#ifndef DEVICE_STATE_MACHINE_H
#define DEVICE_STATE_MACHINE_H

#include "DeviceHandler.h"

// User defined event loops
ESP_EVENT_DECLARE_BASE(STATEMACHINE_EVENTS);

/**
 * The DeviceStateMachine controls the main state machine for humi device. 
 * It is highly recommended to read documentation about StateMachine
 */
class DeviceStateMachine {
public:
    /**
     * Constructor 
     */
    explicit DeviceStateMachine(DeviceHandler& deviceHandler);

    /**
     * Destroy the Device State Machine object
     */
    ~DeviceStateMachine();

    /**
     * Starts the state machine with initial states.
     */
    void start(esp_event_loop_handle_t handle);

    /**
     * runs the state machine. Call this method for running state machine.
     */
    void process();

    /**
     * reset the state machine.
     */
    void initialize();
private:
    /**
     * Represents all states
     */
    enum State {
        INITIAL,
        BOOT,
        SERVICE,
        SERVICE_INITIALIZE,
        CONNECTING,
        RUNNING,
        SLEEP,
        UPDATING,
    };

    void onEnterState(const State state);
    void onRunState(const State state);
    void onLeaveState(const State state);

    void startTimeout(const uint32_t durationMs);

    inline void conditionalStep(const bool conditional, const State valid);
    inline void branchStep(const bool conditional, const State valid, const State invalid);

    // Members
    State currentState;
    State nextState;

    DeviceHandler& handler;
};

#endif // DEVICE_STATE_MACHINE_H
#ifndef DEVICE_STATE_MACHINE_H
#define DEVICE_STATE_MACHINE_H

/**
 * @brief The DeviceStateMachine controls the main state machine for humi device. 
 * It is highly recommended to read documentation about StateMachine
 */
class DeviceStateMachine {
public:
    /**
     * @brief Constructor 
     */
    DeviceStateMachine();

    /**
     * @brief Destroy the Device State Machine object
     */
    ~DeviceStateMachine();

    /**
     * @brief runs the state machine. Call this method for running state machine.
     */
    void process();

    /**
     * @brief reset the state machine.
     */
    void reset();
private:
    /**
     * @brief Represents all states
     */
    enum State {
        INITIAL,
        BOOT,
        SERVICE,
        WIFI_CONNECTING,
        WIFI_CONFIG_HOTSPOT,
        RUNNING,
        SLEEP,
        UPDATING,
    };

    void onEnterState(const State state);
    void onRunState(const State state);
    void onLeaveState(const State state);

    void conditionalStep(const bool isValid, const State state);

    // Members
    State currentState;
    State nextState;
};

#endif // DEVICE_STATE_MACHINE_H
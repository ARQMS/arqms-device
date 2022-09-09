#ifndef CORE_SM_H_
#define CORE_SM_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "Events/WifiSettingsEvent.h"
#include "Events/DeviceInfoEvent.h"

/**
 * Represents the state machine for core control
 */
class CoreSM {
public:
    /**
     * States provided by this state machine
     */
    enum class State {
        BootUp,
        Running,
        Service
    };

    /**
     * Constructor
     */
    explicit CoreSM();

    /**
     * Destructor
     */
    virtual ~CoreSM();

    /**
     * Checks if given state is the current state of SM
     * 
     * @param state to check
     * @return bool true if current state is equal to state; otherwise false
     */
    bool isCurrentState(const State state) const;

    /**
     * Starts service mode
     */
    void onServiceMode();
    
    /**
     * Starts normal mode
     */
    void onRunning();
    
private:
    /**
     * Deleted copy constructor.
     * @param other The copied instance.
     */
    CoreSM(const CoreSM& other) = delete;

    /**
     * Deleted assignment operator.
     * @param other the assignator instance.
     */
    CoreSM& operator=(const CoreSM& other) = delete;

    /**
     * Runs the state machine. Call this method for running the state machine.
     */
    void runStateMachine();

    // state transitions
    void onEnterState(const State state);
    void onRunState(const State state) const;
    void onLeaveState(const State state);
    // transitions
    void handleEvent(bool* const pFlag, const State nextState);

    // state
    State m_currentState;
    State m_nextState;

    bool m_isService;
    bool m_isRunning;
    bool m_isBooting;
};


#endif // CORE_SM_H_
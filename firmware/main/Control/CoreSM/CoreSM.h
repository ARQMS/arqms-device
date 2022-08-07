#ifndef CORE_SM_H_
#define CORE_SM_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "CoreSMIfc.h"
#include "Control/Persistency/NvsStorageLayout.h"

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
        Idle,
        Connecting,
        Service
    };

    /**
     * Constructor
     */
    explicit CoreSM(CoreSMIfc& sender);

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
     * Starts standby mode
     * @param param wifi settings
     */
    void onNormalMode(const WifiParameters& param);

    /**
     * on idle state
     */
    void onConnected();
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

    // Member variables
    CoreSMIfc& m_sender;

    // state
    State m_currentState;
    State m_nextState;

    bool m_isService;
    bool m_isConnecting;
    bool m_isConnected;
    bool m_isConnectionLost;
    bool m_isBooting;

    WifiSettingsEvent m_activeWifiSettings;
};


#endif // CORE_SM_H_
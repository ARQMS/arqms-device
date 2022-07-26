#ifndef WIFI_SM_H_
#define WIFI_SM_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project include
#include "WifiStateMachineIfc.h"

/**
 * Represents the state machine for wifi controller
 */
class WifiStateMachine {
public:
    /**
     * States provided by this state machine
     */
    enum class State {
        OFF,
        SERVICE_WAITING,
        SERVICE,
        NORMAL,
        FAILURE
    };

    /**
     * Constructor
     */
    explicit WifiStateMachine(WifiStateMachineIfc& sender);

    /**
     * Destructor
     */
    virtual ~WifiStateMachine();

    /**
     * Resets the wifi phy
     */
    void reset();

    /**
     * Start service mode
     */
    void onStartServiceMode();

    /**
     * Client connected in service mode
     */
    void onClientConnected();

    /**
     * Client disconnected in service mode
     */
    void onClientDisconnected();

    /**
     * Connection started, but not ready to use. See onWifiConnected
     */
    void onWifiConnecting();

    /**
     * Connected to a external wifi access point
     */
    void onWifiConnected();

    /**
     * Connection list
     */
    void onWifiDisconnected();

    /**
     * Checks if given state is the current state of STM
     * 
     * @param state to check
     * @return bool true if current state is equal to state; otherwise false
     */
    bool isCurrentState(const State state) const;
private:
    /**
     * Deleted copy constructor.
     * @param other The copied instance.
     */
    WifiStateMachine(const WifiStateMachine& other) = delete;

    /**
     * Deleted assignment operator.
     * @param other the assignator instance.
     */
    WifiStateMachine& operator=(const WifiStateMachine& other) = delete;

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
    // helper
    void startWifiAsAp();


    // Member variables
    WifiStateMachineIfc& m_sender;

    // state
    State m_currentState;
    State m_nextState;

    bool m_clientConnected;
    bool m_clientDisonnected;
    bool m_serviceWaiting;
    bool m_normalMode;
    bool m_failure;
};


#endif // WIFI_SM_H_
#ifndef WIFI_SM_H_
#define WIFI_SM_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

/**
 * Represents the state machine for wifi controller
 */
class WifiStateMachine {
public:
    /**
     * States provided by this state machine
     */
    enum class State {
        // Common
        OFF,
        FAILURE,

        // AP
        AP_SERVICE_WAITING,
        AP_SERVICE,

        // STA
        STA_NORMAL_CONNECTING,
        STA_CONNECTED
    };

    /**
     * Constructor
     */
    explicit WifiStateMachine();

    /**
     * Destructor
     */
    virtual ~WifiStateMachine();

    /**
     * Resets the wifi phy
     */
    void reset();

    // State junctions
    void onStaMode();
    void onGotIp();
    void onLostIp();

    void onApMode();
    void onClientConnected();
    void onClientDisconnected();

    void onFailure();

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
    void handleEvent(const bool condition, const State nextState);

    // state
    State m_currentState;
    State m_nextState;

    bool m_failure;

    bool m_startSta;
    bool m_gotIp;
    bool m_lostIp;

    bool m_startAp;
    size_t m_clientCounter;
};


#endif // WIFI_SM_H_
#ifndef WIFI_SM_H_
#define WIFI_SM_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// IDF includes
#include "esp_wifi.h"

// Project include
#include "WifiStateMachineIfc.h"
#include "LocalCtrlHandlerIfc.h"
#include "Events/WifiSettingsEvent.h"

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
        NORMAL_CONNECTING,
        NORMAL,
        FAILURE
    };

    /**
     * Constructor
     */
    explicit WifiStateMachine(WifiStateMachineIfc& sender, LocalCtrlHandlerIfc& ctrlHandler);

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
     * @param wifiSetting wifi settings
     */
    void startServiceMode(const WifiSettingsEvent& wifiSetting);

    /**
     * Normal mode
     * 
     * @param wifiSetting wifi settings
     */
    void startNormalMode(const WifiSettingsEvent& wifiSetting);

    /**
     * Client connected in service mode
     */
    void onClientConnected();

    /**
     * Client disconnected in service mode
     */
    void onClientDisconnected();

    /**
     * Wait for client
     */
    void onClientSearching();

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
    // maximum retry
    const static uint8_t MAXIMUM_RETRY = 5U;

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
    void startWifiAsSta();
    void checkEspError(const esp_err_t status);
    static void onWifiEventHandler(void* param, esp_event_base_t eventBase, int32_t eventId, void* eventData);

    // Member variables
    WifiStateMachineIfc& m_sender;
    LocalCtrlHandlerIfc& m_ctrlHandler;
    WifiSettingsEvent m_wifiSettings;

    // state
    State m_currentState;
    State m_nextState;

    bool m_clientConnected;
    bool m_clientDisonnected;
    bool m_serviceWaiting;
    bool m_connected;
    bool m_connectionLost;
    bool m_normalConnecting;
    bool m_failure;

    uint8_t m_retryNum;
};


#endif // WIFI_SM_H_
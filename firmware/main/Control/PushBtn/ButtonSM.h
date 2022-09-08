#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TimerServiceIfc.h>

#include "Events/EventIdentifiers.h"
#include "Events/ButtonEvent.h"
#include "ButtonSMIfc.h"

/**
 * A software button driver which handles all states and actions fof a single button
 */
class ButtonSM {
public:
    /**
     * Constructor 
     * 
     * @param sender 
     * @param timer 
     * @param id 
     */
    ButtonSM(const ButtonId id, ButtonSMIfc& sender);

    /**
     * Destructor
     */
    ~ButtonSM();

    void initialize(TimerServiceIfc& timerService);

    void onHandleTimer(TimerId timerId);

    void onStateChanged();

private:
    /**
     * Internal states of a button
     */
    enum class State {
        RELEASED,
        WAITING,
        SHORT_PRESSED,
        LONG_PRESSED
    };

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

    ButtonSMIfc& m_sender;
    ButtonId m_id;

    Timer* m_pTimerLongPress;

    // state
    State m_currentState;
    State m_nextState;

    bool m_isLongPressed;
    bool m_stateChanged;
};

#endif // BUTTON_DRIVER_H_
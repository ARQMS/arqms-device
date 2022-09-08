#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TimerServiceIfc.h>

#include "Events/EventIdentifiers.h"
#include "Events/ButtonEvent.h"
#include "ButtonSMIfc.h"

/**
 * A button state machine which handles all states and actions for a single button. Debouncing is done in hardware
 */
class ButtonSM {
public:
    /**
     * Constructor 
     * 
     * @param sender The sm callback interface
     * @param id the button id which represented by this sm
     */
    ButtonSM(const ButtonId id, ButtonSMIfc& sender);

    /**
     * Destructor
     */
    ~ButtonSM();

    /**
     * Button state changed
     */
    void onStateChanged();

private:
    // Constants
    const static uint32_t LONG_PRESS_DURATION_MS = 5000; //< duration of long press in ms

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
    void onLeaveState(const State state);
    // transitions
    void handleEvent(bool* const pFlag, const State nextState);

    ButtonSMIfc& m_sender;
    const ButtonId m_id;
    uint32_t m_pressTick;

    // state
    State m_currentState;
    State m_nextState;

    bool m_isShortPressed;
    bool m_isLongPressed;
    bool m_stateChangePending;
};

#endif // BUTTON_DRIVER_H_
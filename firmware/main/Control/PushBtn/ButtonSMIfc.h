#ifndef BUTTON_DRIVER_IFC_H_
#define BUTTON_DRIVER_IFC_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

#include "Events/ButtonEvent.h"

/**
 * A callback interface to get button state changes
 */
class ButtonSMIfc {
public:
    /**
     * Destructor
     */
    virtual ~ButtonSMIfc() {
    }

    /**
     * Fired whenever a button has been pressed
     * 
     * @param id the button id 
     * @param status the detected status
     */
    virtual void onButtonPressed(const ButtonId id, const ButtonStatus status) = 0;
};

#endif // BUTTON_DRIVER_IFC_H_
#ifndef TIMER_SERVICE_IFC_H_
#define TIMER_SERVICE_IFC_H_

// Platform includes
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>
#include <HumiDevice.Rtos/EventRuntime.h>

// FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

// Project includes
#include "Timer.h"


/**
 * A timer service to create and handle software timers
 */
class TimerServiceIfc {
public:
    /**
     * Destructor
     */
    virtual ~TimerServiceIfc() {};

    /**
     * Starts a periodic timer
     * 
     * @param period of the created timer
     * @return TimerId a unique handle id
     */
    Timer* createPeriodicTimer(const uint32_t period) {
        return EventRuntime::createTimer(getSubscriberId(), period, true);
    }

    /**
     * Starts a one shot timer
     * 
     * @param delay of the created timer
     * @return TimerId a unique handle id
     */
    Timer* createOneShotTimer(const uint32_t delay){
        return EventRuntime::createTimer(getSubscriberId(), delay, false);
    }

    /**
     * Gets the subscribtion identifier
     */
    virtual SubscriberId getSubscriberId() const = 0;
};

#endif // TIMER_SERVICE_IFC_H_
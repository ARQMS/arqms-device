#ifndef TIMER_SERVICE_IFC_H_
#define TIMER_SERVICE_IFC_H_

// Platform includes
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

// FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

/**
 * A unique TimerHandler to control freertos timer. Reuse messagebuffer handler to
 * directly forwared event to correct task
 */
typedef uint8_t TimerId;

/**
 * Unique timer id
 */
static const EventId TimerEventId = 1;

// defines the maximal timer count in application
#define MAX_TIMER 5

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
    TimerId startPeriodicTimer(const uint32_t period);

    /**
     * Starts a one shot timer
     * 
     * @param delay of the created timer
     * @return TimerId a unique handle id
     */
    TimerId startOneShotTimer(const uint32_t delay);

};

#endif // TIMER_SERVICE_IFC_H_
#ifndef EVENT_IDENTIFIERS_H_
#define EVENT_IDENTIFIERS_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>
#include <HumiDevice.Rtos/TimerServiceIfc.h>

/**
 * A list of all available event identifiers 
 */
struct EventIdentifiers {
    const static EventId TEST_EVENT = 0;
    const static EventId TIMER_EVENT = TimerEventId;

    const static EventId QUALITY_EVENT = 100;
};


#endif
#ifndef EVENT_IDENTIFIERS_H_
#define EVENT_IDENTIFIERS_H_

// Project includes
#include <HumiPlatform.h>
#include "Rtos/EventIfc.h"

/**
 * A list of all available event identifiers 
 */
struct EventIdentifiers {
    const static EventId TEST_EVENT = 0;

    const static EventId QUALITY_EVENT = 100;
};


#endif
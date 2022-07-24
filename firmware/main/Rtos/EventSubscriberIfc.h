#ifndef EVENT_SUBSCRIBER_IFC
#define EVENT_SUBSCRIBER_IFC

// Project includes
#include <HumiPlatform.h>
#include "Rtos/EventIfc.h"
#include "Rtos/Serialization/Deserializer.h"

/**
 * An event subscriber which can receive any events
 */
class EventSubscriberIfc {
public:
    /**
     * Destructor
     */
    virtual ~EventSubscriberIfc() {};


    /**
     * Called whenever a event is received to this task
     * 
     * @param eventID the unique event id
     * @param pEvent the optional data
     */
    virtual void execute(EventId eventId, Deserializer* pEvent = NULL) = 0;
};

#endif // EVENT_SUBSCRIBER_IFC
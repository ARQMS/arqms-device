#ifndef EVENT_PUBLISHER_IFC_
#define EVENT_PUBLISHER_IFC_

// Project includes
#include <HumiPlatform.h>
#include "EventIfc.h"

/**
 * An event publisher which sends any events to connected tasks 
 */
class EventPublisherIfc {
public:
  
    /**
     * Destructor
     */
    virtual ~EventPublisherIfc() {};

    /**
     * Sends the optional event to connected tasks.
     * 
     * @param eventId the unique event id
     * @param pEvent the optional data
     */
    virtual void send(EventId eventId, EventIfc* pEvent = NULL) = 0;
};


#endif // EVENT_PUBLISHER_IFC_
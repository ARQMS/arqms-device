#ifndef EVENT_SUBSCRIBER_IFC
#define EVENT_SUBSCRIBER_IFC

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Project includes
#include <HumiPlatform.h>
#include "Rtos/EventIfc.h"


/**
 * A unique task identifier used to route @see EventIfc through a queue, into the appropriate tasks.
 */
typedef uint32_t SubscriberId;

/**
 * Represent a invalid subscriber id. All Subscriber identifiers greater than this are valid
 */
static const SubscriberId INVALID_SUBSCRIBER_ID = -1;

// TODO [sanore] Create a TaskIdGenerator class and increase id thread-safe
/**
 * A counter to get unique task id. Attention this is not thread-safe
 */
static SubscriberId s_subCounter = INVALID_SUBSCRIBER_ID;

/**
 * An event subscriber which can receive any events
 */
class EventSubscriberIfc {
public:
    /**
     * Constructor
     */
    EventSubscriberIfc() : 
        m_subscriberId(++s_subCounter) {
    }

    /**
     * Destructor
     */
    virtual ~EventSubscriberIfc() {};
  
    /**
     * Gets the associated task id
     * @return const TaskId 
     */
    SubscriberId getId() const {
        return m_subscriberId;
    }

protected:

    /**
     * Executs the received event
     */
    virtual void onExecute(EventId eventId, EventIfc* pEvent = NULL) = 0;

private:

    // Members
    const SubscriberId m_subscriberId;
};

#endif // EVENT_SUBSCRIBER_IFC
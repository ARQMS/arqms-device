#ifndef EVENT_SINGLE_PUBLISHER_H_
#define EVENT_SINGLE_PUBLISHER_H_

// include FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// project includes
#include <HumiPlatform.h>
#include "EventPublisherIfc.h"
#include "EventSubscriberIfc.h"

/**
 * A simple event publisher which can be connected to single target subscriber.
 */
class EventSinglePublisher : public EventPublisherIfc {
public:
    /**
     * Constructor
     */
    EventSinglePublisher() {};

    /**
     * Destructor
     */
    virtual ~EventSinglePublisher() {};

    /**
     * Connects the given subscriber to this publisher
     * 
     * @param subscriber to send events when send() is called
     */
    void connect(EventSubscriberIfc& subscriber) {
        m_subId = subscriber.getId();
    }

    /**
     * @see EventPublisherIfc::send()
     */
    virtual void send(EventId eventId, EventIfc* pEvent = NULL) override {
        if (m_subId <= INVALID_SUBSCRIBER_ID) {
            // TODO error
        }

        // TODO serialize data. Use Protobuf?
        // data << eventId;
        // pEvent?.serialize(eventId);

        /*if (xQueueSend(m_subId, &data, 100) != pdPASS) {
            // TODO not sent, log message
        }*/
    }

private:
    SubscriberId m_subId;
};

#endif
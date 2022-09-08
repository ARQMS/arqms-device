#ifndef EVENT_SINGLE_PUBLISHER_H_
#define EVENT_SINGLE_PUBLISHER_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// include FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// project includes
#include "EventRuntime.h"
#include "EventPublisherIfc.h"
#include "TaskIfc.h"

/**
 * A simple event publisher which can be connected to single target subscriber.
 */
class EventPublisherSingle : public EventPublisherIfc {
public:
    /**
     * Constructor
     */
    EventPublisherSingle() {};

    /**
     * Destructor
     */
    virtual ~EventPublisherSingle() {};

    /**
     * Connects the given subscriber to this publisher
     * 
     * @param subscriber to send events when send() is called
     */
    void connect(TaskIfc& subscriber) {
        m_subId = subscriber.getSubscriberId();
    }

    /**
     * @see EventPublisherIfc::send()
     */
    virtual void send(EventId eventId, EventIfc* pEvent = NULL) override {
        if (m_subId <= INVALID_SUBSCRIBER_ID) {
            ESP_DRAM_LOGE("HumiDevice", "Subscriber is invalid. No events sent");
            return;
        }

        EventRuntime::send(m_subId, eventId, pEvent);
    }

private:
    SubscriberId m_subId;
};

#endif
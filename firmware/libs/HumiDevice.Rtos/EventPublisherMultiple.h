#ifndef EVENT_MULTIPLE_PUBLISHER_H_
#define EVENT_MULTIPLE_PUBLISHER_H_

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
 * A event publisher which can be connected to multiple target subscriber.
 * 
 * @tparam SIZE number of expected subscribers
 */
template<size_t SIZE>
class EventPublisherMultiple : public EventPublisherIfc {
public:
    /**
     * Constructor
     */
    EventPublisherMultiple() {};

    /**
     * Destructor
     */
    virtual ~EventPublisherMultiple() {};

    /**
     * Connects the given subscriber to this publisher
     * 
     * @param subscriber to send events when send() is called
     */
    void connect(TaskIfc& subscriber) {
        bool error = true;
        for (size_t idx = 0; idx < SIZE; ++idx) {
            // in use
            if (m_subIds[idx] != NULL) continue;

            // free slot found, no error abort loop
            m_subIds[idx] = subscriber.getSubscriberId();
            error = false;
            break;
        }

        if (error) {
            ESP_LOGE("HumiDevice", "%i slots for EventPublisherMultiple is not enough", SIZE);
        }
    }

    /**
     * @see EventPublisherIfc::send()
     */
    virtual void send(EventId eventId, EventIfc* pEvent = NULL) override {
        for (size_t idx = 0; idx < SIZE; ++idx) {
            if (m_subIds[idx] != NULL) {
                EventRuntime::send(m_subIds[idx], eventId, pEvent);
            }
        }
    }

private:
    SubscriberId m_subIds[SIZE];
};

#endif
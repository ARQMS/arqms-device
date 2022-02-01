#ifndef EVENTLOOP_IFC_H
#define EVENTLOOP_IFC_H

#include "HumiPlatform.h"
#include "esp_event.h"

typedef int16_t TimerId;

/**
 * Sends event to application loop
 */
class EventLoopIfc {
public:
    /**
     * Deconstructor
     */
    ~EventLoopIfc() { }

    /**
     * Used for sending event from task context to application loop
     * 
     * @param eventBase the event base
     * @param eventId the event id
     * @param data the optional data
     * @param dataSize the data size to send
     */
    virtual void sendEvent(esp_event_base_t eventBase, int32_t eventId, void* data = nullptr, size_t dataSize = 0) = 0;

    /**
     * Used for sending event from ISR context to application loop
     * 
     * @param eventBase the event base
     * @param eventId the event id
     * @param data the optional data
     * @param dataSize the data size to send
     */
    virtual void sendEventIsr(esp_event_base_t eventBase, int32_t eventId, void* data = nullptr, size_t dataSize = 0) = 0;

    /**
     * Starts a new timer when id does not exists. Otherwise duration is reseted
     * @param id 
     * @param durationMs 
     */
    virtual TimerId startOneShotTimer(uint32_t durationMs) = 0;
};

#endif // EVENTLOOP_IFC_H
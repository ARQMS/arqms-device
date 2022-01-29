#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "Platform.h"
#include "EventHandlerIfc.h"
#include "EventLoopIfc.h"

/**
 * A EventDispatcher which uses the ESP-IDF event loop framework to notify HMI about changes.
 */
class EventDispatcher : public EventLoopIfc {
public:

    /**
     * Constructor
     */
    explicit EventDispatcher(void);

    /**
     * Deconstructor
     */
    ~EventDispatcher() { }

    /**
     * Initializes the application loop
     */
    void initialize();

    /**
     * @see EventLoopIfc
     */
    void sendEvent(esp_event_base_t eventBase, int32_t eventId, void* data = nullptr, size_t dataSize = 0) override;

    /**
     * @see EventLoopIfc
     */
    void sendEventIsr(esp_event_base_t eventBase, int32_t eventId, void* data = nullptr, size_t dataSize = 0) override;

    /**
     * @see EventLoopIfc
     */
    TimerId startOneShotTimer(uint32_t durationMs) override;

    /**
     * Register the given event base and event id for handler
     * 
     * @param eventBase the event base
     * @param eventId the event id
     * @param handler the handler
     */
    void registerEventHandler(esp_event_base_t eventBase, int32_t eventId, EventHandlerIfc& handler);
private:
    /**
     * Private copy constructor.
     */
    EventDispatcher(const EventDispatcher& other);

    /**
     * Private assignment operator.
     */
    EventDispatcher& operator=(const EventDispatcher& other);

    // members
    static esp_event_loop_handle_t s_appLoopHandle;
    static uint32_t s_timerIdx;
};

#endif // EVENTDISPATCHER_H
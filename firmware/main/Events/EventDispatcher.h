#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

// ESP-IDF
#include "esp_event.h"

// Application
#include "Platform.h"
#include "EventHandlerIfc.h"

/**
 * A EventDispatcher which uses the ESP-IDF event loop framework to notify HMI about changes.
 */
class EventDispatcher {
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
     * 
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
};

#endif // EVENTDISPATCHER_H
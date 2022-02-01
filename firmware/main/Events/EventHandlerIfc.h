#ifndef EVENTHANDLERIFC_H
#define EVENTHANDLERIFC_H

#include "HumiPlatform.h"
#include "esp_event.h"

/**
 * Callback interface for IDF framework. Maps c function to c++ class member function
 */
class EventHandlerIfc {
public:
    /**
     * Deconstructor
     */
    ~EventHandlerIfc() { }

    /**
     * Called whenever an event is received for this handler.
     * 
     * @param base event base
     * @param id evnt id
     * @param data according data, can be null
     */
    virtual void onEvent(esp_event_base_t base, int32_t id, void* data) = 0;
};

#endif // EVENTHANDLERIFC_H
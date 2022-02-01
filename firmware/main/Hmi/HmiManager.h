#ifndef HMIMANAGER_H
#define HMIMANAGER_H

#include "HumiPlatform.h"
#include "Events/EventHandlerIfc.h"
#include "Events/EventLoopIfc.h"

/**
 * The human-machine interface manager to control all the leds and buttons.
 */
class HmiManager : public EventHandlerIfc {
public:
    /**
     * Constructor
     */
    explicit HmiManager(EventLoopIfc& eventLoop);

    /**
     * Deconstructor
     */
    ~HmiManager() { }

    /**
     * Initializes all the components to default state 
     */
    void initialize();

    /**
     * @see EventHandlerIfc
     */
    void onEvent(esp_event_base_t base, int32_t id, void* data) override;
    
private:
    /**
     * Private copy constructor.
     */
    HmiManager(const HmiManager& other);

    /**
     * Private assignment operator.
     */
    HmiManager& operator=(const HmiManager& other);

    // private helper functions
    void onUsrPressed();
    void onResetTimer();

    // Members
    EventLoopIfc& eventLoop;
    TimerId resetTimerId;
};

#endif // HMIMANAGER_H
#ifndef HMIMANAGER_H
#define HMIMANAGER_H

#include "Platform.h"
#include "esp_event.h"
#include "Events/EventHandlerIfc.h"

/**
 * The human-machine interface manager to control all the leds and buttons.
 */
class HmiManager : public EventHandlerIfc {
public:
    /**
     * Constructor
     */
    explicit HmiManager();

    /**
     * Deconstructor
     */
    ~HmiManager() { }

    /**
     * Initializes all the components to default state 
     */
    void initialize();

    /**
     * Updates the hmi according to current state
     */
    void update();

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
};

#endif // HMIMANAGER_H
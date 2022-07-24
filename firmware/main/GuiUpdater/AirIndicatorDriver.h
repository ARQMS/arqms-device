#ifndef AIR_INDICATOR_DRIVER_H_ 
#define AIR_INDICATOR_DRIVER_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// IDF includes
#include "driver/gpio.h"

// Project includes
#include "Events/AirQualityEvent.h"

/**
 * AirIndicatorDriver manages air indicator
 */
class AirIndicatorDriver {
public:
    /**
     * Construtor
     */
    AirIndicatorDriver(const gpio_num_t goodPin, const gpio_num_t modPin, const gpio_num_t poorPin);

    /**
     * Destructor
     */
    ~AirIndicatorDriver();

    /**
     * Set the air quality
     * 
     * @param quality is used for led indication
     */
    void setQuality(const AirQuality quality);

private:
    /**
     * Private default constructor
     */
    AirIndicatorDriver(void);

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    AirIndicatorDriver(const AirIndicatorDriver& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    AirIndicatorDriver& operator=(const AirIndicatorDriver& other);

    // Private Members
    const gpio_num_t m_goodPin;
    const gpio_num_t m_modPin; 
    const gpio_num_t m_poorPin; 

    // LED Helper state
    static const uint8_t ON     = 0;
    static const uint8_t OFF    = 1;
};


#endif // AIR_INDICATOR_DRIVER_H_

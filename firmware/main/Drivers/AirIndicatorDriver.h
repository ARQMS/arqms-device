#ifndef AIR_INDICATOR_DRIVER_H_ 
#define AIR_INDICATOR_DRIVER_H_

// IDF includes
#include "driver/gpio.h"

// Project includes
#include <HumiPlatform.h>

/**
 * AirIndicatorDriver manages air indicator
 */
class AirIndicatorDriver {
public:
    /**
     * Enum for air quality 
     */
    enum Quality {
        POOR,
        MOD,
        GOOD,
        UNKNOWN
    };

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
    void setQuality(const AirIndicatorDriver::Quality quality);

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
};


#endif // AIR_INDICATOR_DRIVER_H_
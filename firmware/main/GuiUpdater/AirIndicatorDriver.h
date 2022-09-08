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
     * @param quality is used for led indication.     
     *          - 0 means bad quality
     *          - 1 means best quality
     */
    void setQuality(const float32_t quality);

private:

    // Enum for air quality 
    enum AirQuality {
        UNKNOWN = 0,
        GOOD    = 1,
        MOD     = 2,
        POOR    = 3,
    };

    // Constants
    const static float32_t GOOD_THRESHOLD;
    const static float32_t MOD_THRESHOLD;
    const static uint8_t ON;
    const static uint8_t OFF;

    // Helper method
    AirQuality calculateAirQuality(const float32_t quality);

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

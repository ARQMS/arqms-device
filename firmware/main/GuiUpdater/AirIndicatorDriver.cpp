#include "AirIndicatorDriver.h"

AirIndicatorDriver::AirIndicatorDriver(const gpio_num_t goodPin, const gpio_num_t modPin, const gpio_num_t poorPin) :
    m_goodPin(goodPin),
    m_modPin(modPin),
    m_poorPin(poorPin) {
}

AirIndicatorDriver::~AirIndicatorDriver() {
}

void AirIndicatorDriver::setQuality(const AirQuality quality) {
    switch (quality) {
        case AirQuality::GOOD: 
            gpio_set_level(m_goodPin, ON);
            gpio_set_level(m_modPin, OFF);
            gpio_set_level(m_poorPin, OFF);
            break;

        case AirQuality::MOD: 
            gpio_set_level(m_goodPin, ON);
            gpio_set_level(m_modPin, ON);
            gpio_set_level(m_poorPin, OFF);
            break;

        case AirQuality::POOR: 
            gpio_set_level(m_goodPin, ON);
            gpio_set_level(m_modPin, ON);
            gpio_set_level(m_poorPin, ON);
            break;

        default:
            gpio_set_level(m_goodPin, OFF);
            gpio_set_level(m_modPin, OFF);
            gpio_set_level(m_poorPin, OFF);
            break;
    }
}
#include "AirIndicatorDriver.h"

AirIndicatorDriver::AirIndicatorDriver(const gpio_num_t goodPin, const gpio_num_t modPin, const gpio_num_t poorPin) :
    m_goodPin(goodPin),
    m_modPin(modPin),
    m_poorPin(poorPin) {
}

AirIndicatorDriver::~AirIndicatorDriver() {
}

void AirIndicatorDriver::setQuality(const AirIndicatorDriver::Quality quality) {
    switch (quality) {
        case Quality::GOOD: 
            gpio_set_level(m_goodPin, 1);
            gpio_set_level(m_modPin, 0);
            gpio_set_level(m_poorPin, 0);
            break;

        case Quality::MOD: 
            gpio_set_level(m_goodPin, 1);
            gpio_set_level(m_modPin, 1);
            gpio_set_level(m_poorPin, 0);
            break;

        case Quality::POOR: 
            gpio_set_level(m_goodPin, 1);
            gpio_set_level(m_modPin, 1);
            gpio_set_level(m_poorPin, 1);
            break;

        default:
            gpio_set_level(m_goodPin, 0);
            gpio_set_level(m_modPin, 0);
            gpio_set_level(m_poorPin, 0);
            break;
    }
}

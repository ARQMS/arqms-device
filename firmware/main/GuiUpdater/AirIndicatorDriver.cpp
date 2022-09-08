#include "AirIndicatorDriver.h"

// Const initialization
const float32_t AirIndicatorDriver::GOOD_THRESHOLD  = 0.66f;
const float32_t AirIndicatorDriver::MOD_THRESHOLD   = 0.33f;
const uint8_t AirIndicatorDriver::ON                = 0;
const uint8_t AirIndicatorDriver::OFF               = 1;

AirIndicatorDriver::AirIndicatorDriver(const gpio_num_t goodPin, const gpio_num_t modPin, const gpio_num_t poorPin) :
    m_goodPin(goodPin),
    m_modPin(modPin),
    m_poorPin(poorPin) {
        setQuality(-1.f);
}

AirIndicatorDriver::~AirIndicatorDriver() {
}

void AirIndicatorDriver::setQuality(const float32_t quality) {
    AirQuality indicator = calculateAirQuality(quality);

    switch (indicator) {
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

AirIndicatorDriver::AirQuality AirIndicatorDriver::calculateAirQuality(const float32_t quality) {
    if (quality >= GOOD_THRESHOLD) return AirQuality::GOOD;
    else if (quality >= MOD_THRESHOLD) return AirQuality::MOD;
    else if(quality < 0.f) return AirQuality::UNKNOWN;
    else return AirQuality::POOR;
}
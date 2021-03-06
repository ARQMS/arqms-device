#include "AirIndicatorDriver.h"

// Const initialization
const float32_t AirIndicatorDriver::POOR_THRESHOLD  = 0.8f;
const float32_t AirIndicatorDriver::MOD_THRESHOLD   = 0.5f;
const uint8_t AirIndicatorDriver::ON                = 0;
const uint8_t AirIndicatorDriver::OFF               = 1;

AirIndicatorDriver::AirIndicatorDriver(const gpio_num_t goodPin, const gpio_num_t modPin, const gpio_num_t poorPin) :
    m_goodPin(goodPin),
    m_modPin(modPin),
    m_poorPin(poorPin) {
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
    if (quality >= POOR_THRESHOLD) return AirQuality::POOR;
    else if (quality >= MOD_THRESHOLD) return AirQuality::MOD;
    else return AirQuality::GOOD;
}
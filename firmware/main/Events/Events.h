#ifndef HMIEVENTS_H
#define HMIEVENTS_H

// ESP-IDF
#include "esp_event.h"

#ifdef __cplusplus
extern "C" {
#endif

ESP_EVENT_DECLARE_BASE(HMI_EVENTS);
ESP_EVENT_DECLARE_BASE(SENSOR_EVENTS);
ESP_EVENT_DECLARE_BASE(TIMER_EVENTS);

/**
 * A list of supported HMI events
 */
enum AppEvents {
    // Common
    TIMER_ELAPSED,
    TIMER_STARTED,

    // HMI
    USR_BTN_PRESSED,
    RST_BTN_PRESSED,

    // Sensor
    DATA_ACQUIRED,
};


#ifdef __cplusplus
}
#endif

#endif // HMIEVENTS_H
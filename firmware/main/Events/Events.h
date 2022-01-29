#ifndef HMIEVENTS_H
#define HMIEVENTS_H

// ESP-IDF
#include "esp_event.h"

#ifdef __cplusplus
extern "C" {
#endif

ESP_EVENT_DECLARE_BASE(HMI_EVENTS);
ESP_EVENT_DECLARE_BASE(SENSOR_EVENTS);

/**
 * A list of supported HMI events
 */
enum HmiEvents {
    USR_BTN_PRESSED,
    RST_BTN_PRESSED,
};

/**
 * A list of supported sensor events.
 */
enum SensorEvents {
    DATA_ACQUIRED,
};

#ifdef __cplusplus
}
#endif

#endif // HMIEVENTS_H
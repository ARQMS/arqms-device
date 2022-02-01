#ifndef HMIEVENTS_H
#define HMIEVENTS_H

// ESP-IDF
#include "HumiPlatform.h"
#include "esp_event.h"

#ifdef __cplusplus
extern "C" {
#endif

ESP_EVENT_DECLARE_BASE(HMI_EVENTS);
ESP_EVENT_DECLARE_BASE(SENSOR_EVENTS);
ESP_EVENT_DECLARE_BASE(TIMER_EVENTS);
ESP_EVENT_DECLARE_BASE(DEVICE_EVENTS);

/**
 * A list of supported events
 */
enum AppEvents {
    // Common
    TIMER_ELAPSED,          // no Data
    TIMER_STARTED,          // no Data

    // Device
    NEW_FIRMWARE_FOUND,     // see NewFirmwareInfo
    CONFIG_CHANGED,         // see DeviceConfigInfo

    // HMI
    USR_BTN_PRESSED,        // no Data
    RST_BTN_PRESSED,        // no Data

    // Sensor
    DATA_ACQUIRED,          // see SensorData
};

struct SensorData {
    float32_t relativeHumidity;
};

/**
 * Structure with new firmware information.
 */
struct DeviceFirmwareInfo {
    // TODO Download URL
    // Version Number
};

/**
 * Structure with new configuration
 */
struct DeviceConfigInfo {

};

#ifdef __cplusplus
}
#endif

#endif // HMIEVENTS_H
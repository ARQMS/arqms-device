#ifndef EVENT_IDENTIFIERS_H_
#define EVENT_IDENTIFIERS_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>
#include <HumiDevice.Rtos/Timer.h>

/**
 * A list of all available event identifiers 
 */
struct EventIdentifiers {
    // Internal Rtos 0..10
    const static EventId TEST_EVENT             = 0;
    const static EventId TIMER_EVENT            = TimerEventId;

    // DEVICE 60..79
    const static EventId DEVICE_SETTINGS_EVENT  = 60;
    const static EventId SENSOR_DATA_EVENT      = 61;
    const static EventId BATTERY_LEVEL_EVENT    = 62;

    // WIFI 80..99
    const static EventId WIFI_SETTINGS_EVENT    = 80;
    const static EventId WIFI_STATUS_EVENT      = 81;

    // GUI 100..120
    const static EventId QUALITY_EVENT          = 100;

    // reserve 120..127
};


#endif

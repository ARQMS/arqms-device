#ifndef TIMER_H_
#define TIMER_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// FreeRTOS includes
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

// Project includes
#include "TaskIfc.h"

/**
 * A unique TimerHandler to control freertos timer. Reuse messagebuffer handler to
 * directly forwared event to correct task
 */
typedef uint8_t TimerId;

/**
 * Unique timer id
 */
static const EventId TimerEventId = 1;

// internal timer structure
struct Timer {
    StaticTimer_t internal;
    TimerHandle_t handle;
    SubscriberId taskHandle;
    TimerId id;

    /**
     * Starts the given timer
     */
    void start() {
        if (xTimerIsTimerActive(handle) == pdFALSE) {
            // start timer, do not block
            xTimerStart(handle, 0);
        }
    }

    /**
     * Stops the given timer
     */
    void stop() {
        // stop timer, do not block
        xTimerStop(handle, 0);
    }
};

#endif 
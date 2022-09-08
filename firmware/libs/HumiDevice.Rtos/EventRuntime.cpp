#include "EventRuntime.h"

// Platform
#include <HumiDevice.Serialization/Deserializer.h>
#include <HumiDevice.Serialization/Serializer.h>

#include "TimerEvent.h"

// global variables
static TimerId g_timerCounter = 0;
static Timer g_timerBuffer[MAX_TIMER];

extern "C" void taskProcess(void* parameter) {
    TaskIfc* pTask = static_cast<TaskIfc*>(parameter);
    if (pTask == NULL) return;

    // suspend current task to ensure task is fully initialized and connected to
    // other tasks. It may happen getTaskId() is nut fully initialized, so ensure suspend
    // is called befor any method is called from pTask
    vTaskSuspend(NULL);
    
    const TaskId taskHandle = pTask->getTaskId();
    const SubscriberId queueHandle = pTask->getSubscriberId();
    static uint8_t dataStream[MAX_MESSAGE_SIZE];

    ESP_LOGV("HumiDevice", "Task %p initialized...", taskHandle);

    pTask->start();

    ESP_LOGV("HumiDevice", "Task %p started...", taskHandle);

    do {
        // Block thread until any event is received. weak at least every second to check shutdown request
        size_t receivedBytes = xMessageBufferReceive(queueHandle, (void*)dataStream, MAX_MESSAGE_SIZE, portMAX_DELAY);
        if (receivedBytes > 0) {
            EventRuntime::process(*pTask, dataStream, receivedBytes);
        }
    } while (!pTask->isShutdownRequested());
}

extern "C" void taskTimer(TimerHandle_t parameter) {
    TimerId& timerId = *(TimerId*) pvTimerGetTimerID(parameter);

    ESP_LOGV("HumiDevice", "TimerId %i fired in %p", timerId, g_timerBuffer[timerId].taskHandle);

    TimerEvent data(timerId);
    EventRuntime::send(g_timerBuffer[timerId].taskHandle, TimerEventId, &data);
}

void EventRuntime::send(const SubscriberId handle, const EventId id, const EventIfc* const pData) {
    static uint8_t dataStream[MAX_MESSAGE_SIZE];

    Serializer serializer(dataStream, MAX_MESSAGE_SIZE);
    serializer << id;
    if (pData != NULL) {
        pData->serialize(serializer);
    }

    if (xMessageBufferSendFromISR(handle, (void*)dataStream, serializer.getBufferPos(), 0) == 0) {
        ESP_LOGE("HumiDevice", "Queue full. No eventid %i to %p sent", id, handle);
    }
}

void EventRuntime::process(TaskIfc& task, const void* pData, const size_t dataLength) {
    uint8_t* dataStream = (uint8_t*)pData;

    Deserializer deserializer(dataStream, dataLength);
    EventId id;

    deserializer >> id;

    task.execute(id, &deserializer);
}

Timer* EventRuntime::createTimer(const SubscriberId subId, const uint32_t period, bool isPeriodic) {
    if (g_timerCounter + 1 > MAX_TIMER) {
        ESP_LOGE("HumiDevice", "EventRuntime out of Timer. Increase MAX_TIMER define!");
        return NULL;
    }

    // next timer, not thread-safe yet!
    const TimerId timerId = g_timerCounter++;
    const UBaseType_t autoReload = isPeriodic ? pdTRUE : pdFALSE;

    // save id in seperate register to avoid race-conditions
    g_timerBuffer[timerId].id = timerId;
    g_timerBuffer[timerId].taskHandle = subId;
    g_timerBuffer[timerId].handle = xTimerCreateStatic("Timer", period / portTICK_PERIOD_MS, autoReload, &g_timerBuffer[timerId].id, taskTimer, &g_timerBuffer[timerId].internal);

    return &g_timerBuffer[timerId];
}

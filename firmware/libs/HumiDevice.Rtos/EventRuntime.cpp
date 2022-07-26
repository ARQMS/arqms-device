#include "EventRuntime.h"

// Platform
#include <HumiDevice.Serialization/Deserializer.h>
#include <HumiDevice.Serialization/Serializer.h>

TimerId EventRuntime::s_timerCounter = 0;

extern "C" void taskProcess(void* parameter) {
    TaskIfc* pTask = static_cast<TaskIfc*>(parameter);
    if (pTask == NULL) return;

    // suspend current task to ensure task is fully initialized and connected to
    // other tasks. It may happen getTaskId() is nut fully initialized, so ensure suspend
    // is called befor any method is called from pTask
    vTaskSuspend(NULL);
    
    const TaskId taskHandle = pTask->getTaskId();
    const SubscriberId queueHandle = pTask->getSubscriberId();
    uint8_t dataStream[MAX_MESSAGE_SIZE];

    ESP_LOGI("HumiDevice", "Task %p initialized...", taskHandle);

    pTask->start();

    ESP_LOGI("HumiDevice", "Task %p started...", taskHandle);

    do {
        // Block thread until any event is received. weak at least every second to check shutdown request
        size_t receivedBytes = xMessageBufferReceive(queueHandle, (void*)dataStream, MAX_MESSAGE_SIZE, 1000 / portTICK_PERIOD_MS);
        EventRuntime::process(*pTask, dataStream, receivedBytes);
    } while (!pTask->isShutdownRequested());
}

extern "C" void taskTimer(TimerHandle_t parameter) {
    SubscriberId* pSubId = (SubscriberId*)parameter;
    EventRuntime::send(*pSubId, TimerEventId);
}

void EventRuntime::send(const SubscriberId handle, const EventId id, const EventIfc* const pData) {
    uint8_t dataStream[MAX_MESSAGE_SIZE];

    Serializer serializer(dataStream, MAX_MESSAGE_SIZE);
    serializer << id;
    if (pData != NULL) {
        pData->serialize(serializer);
    }

    ESP_LOGI("HumiDevice", "Sending ID %i to %p", id, handle);

    if (xMessageBufferSend(handle, (void*)dataStream, serializer.getBufferPos(), 0) == 0) {
        ESP_LOGE("HumiDevice", "Queue full. No events sent");
    }
}

void EventRuntime::process(TaskIfc& task, const void* pData, const size_t dataLength) {
    uint8_t* dataStream = (uint8_t*)pData;

    Deserializer deserializer(dataStream, dataLength);
    EventId id;

    deserializer >> id;

    ESP_LOGI("HumiDevice", "Received ID %i in %p", id, task.getSubscriberId());

    task.execute(id, &deserializer);
}

TimerId EventRuntime::startTimer(const SubscriberId subId, const uint32_t period, bool isPeriodic) {
    if (s_timerCounter >= MAX_TIMER) {
        ESP_LOGE("HumiDevice", "EventRuntime out of Timer. Increase MAX_TIMER define!");
        return s_timerCounter + 1; // return an invalid timerId
    }

    const UBaseType_t autoReload = isPeriodic ? pdTRUE : pdFALSE;
    s_xTimerId[s_timerCounter] = xTimerCreateStatic("Timer", period / portTICK_PERIOD_MS, autoReload, (void*)subId, taskTimer, &s_xTimerStruct[s_timerCounter]);

    // start timer immediatly
    xTimerStart(s_xTimerId[s_timerCounter], 0);

    return s_timerCounter++;
}

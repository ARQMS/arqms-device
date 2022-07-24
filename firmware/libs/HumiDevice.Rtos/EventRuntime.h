#ifndef TASK_FACTORY_H_
#define TASK_FACTORY_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// FreeRTOS includes
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/message_buffer.h>

// Project includes
#include "TaskIfc.h"

/**
 * Task process method, it's called in seperated context for each task
 */
extern "C" void taskProcess(void* parameter);

/**
 * defines FreeRTOS structure and task object and register 
 * task in scheduler. 
 * 
 * @param name The name must be the Task type without Task as prefix
 * @param stackSize the maximal stack size in bytes
 */
#define CREATE_TASK_DEF(name, stackSize, priority) \
    static name## Task name; \
    StaticTask_t x ##name## TaskBuffer; \
    StackType_t x ##name## TaskStack[stackSize]; \
    name## Task* create ##name## Task() {\
        TaskId taskId = xTaskCreateStatic( \
            taskProcess,  \
            #name,  \
            stackSize,  \
            (void*)&name,  \
            priority, \
            x ##name## TaskStack, \
            &x ##name## TaskBuffer \
        ); \
        name.initialize(taskId); \
        return &name; \
    } \
    void start ##name## Task() {\
        vTaskResume(name.getTaskId()); \
    } \

/**
 * Wrapper for sending receiving messages to FreeRTOS queue system
 */
class EventRuntime {
public:
    /**
     * Sends a event to given handle
     * 
     * @param handle The target handle to send event to
     * @param id The unique event id
     * @param pData The optional event data
     */
    static void send(const SubscriberId handle, const EventId id, const EventIfc* pData = NULL);

    /**
     * Called when any message is received 
     *  
     * @param task The task which receives the message
     * @param pData The raw values
     * @param dataLength The total length of pData
     */
    static void process(TaskIfc& task, const void* pData, const size_t dataLength);
private:
    EventRuntime();
    ~EventRuntime();
};



#endif // TASK_FACTORY_H_
#ifndef TASK_FACTORY_H_
#define TASK_FACTORY_H_

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Project includes
#include "Rtos/TaskIfc.h"

/**
 * Task process method, it's called in seperated context for each task
 */
extern "C" void taskProcess(void* parameter) {
    TaskIfc* pTask = static_cast<TaskIfc*>(parameter);
    if (pTask == NULL) return;

    pTask->initialize();
    pTask->start();
}

/**
 * defines FreeRTOS structure and task object and register 
 * task in scheduler. 
 * 
 * @param name The name must be the Task type without Task as prefix
 * @param stackSize the maximal stack size in bytes
 */
#define CREATE_TASK_DEF(name, stackSize) \
    static name## Task name; \
    StaticTask_t x ##name## TaskBuffer; \
    StackType_t x ##name## TaskStack[stackSize]; \
    name## Task* create ##name## Task() {\
        xTaskCreateStatic( \
            taskProcess,  \
            "Task",  \
            stackSize,  \
            (void*)&name,  \
            tskIDLE_PRIORITY, \
            x ##name## TaskStack, \
            &x ##name## TaskBuffer \
        ); \
        return &name; \
    } \


#endif // TASK_FACTORY_H_
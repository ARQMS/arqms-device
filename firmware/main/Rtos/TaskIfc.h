#ifndef TASKIFC_H_
#define TASKIFC_H_

// Project includes
#include <HumiPlatform.h>

/**
 * Interface for FreeRTOS Task.
 * 
 * It wrappes framework FreeRTOS Task into type-safe task. Tasks are an unit which encapsulate
 * the state and behavior from other tasks. It interconnects to other tasks with queues.
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/freertos.html#task-api
 */
class TaskBase {
public:
    /**
     * Destructor
     */
    virtual ~TaskBase() {}
};


#endif // TASKIFC_H_
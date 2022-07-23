#ifndef TASK_BASE_H_
#define TASK_BASE_H_

// Project includes
#include <HumiPlatform.h>

#include "Rtos/TaskIfc.h"
#include "Rtos/EventSubscriberIfc.h"
#include "Rtos/EventPublisherIfc.h"

/**
 * Wrapper for FreeRTOS Task.
 * 
 * It wrappes framework FreeRTOS Task into type-safe task. Tasks are an unit which encapsulate
 * the state and behavior from other tasks. It interconnects to other tasks with queues.
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/freertos.html#task-api
 */
class TaskBase : public TaskIfc {
public:
    /**
     * Constructor 
     */
    TaskBase();

    /**
     * Destructor
     */
    virtual ~TaskBase();

    /**
     * @see TaskIfc::initialize()
     */
    void initialize();

    /**
     * @see TaskIfc::start()
     */
    void start();
protected:
    /**
     * Initialize the task
     */
    virtual void onInitialize() = 0;

    /**
     * Starts the task
     */
    virtual void onStart() = 0;

    /**
     * @see EventSubscriberIfc::onExecute()
     */
    virtual void onExecute(EventId eventId, EventIfc* pEvent = NULL) = 0;

private:
    bool m_isInitialized;
    bool m_isStarted;
};


#endif // TASK_BASE_H_
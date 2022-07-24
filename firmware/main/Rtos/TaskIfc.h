#ifndef TASKIFC_H_
#define TASKIFC_H_

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"

// Project includes
#include <HumiPlatform.h>

#include "Rtos/EventSubscriberIfc.h"
#include "Rtos/EventIfc.h"

// A unique task identifier used to control task from FreeRTOS
typedef TaskHandle_t TaskId;

// A unique message queue identifier used to route @see EventIfc through a queue, into the appropriate tasks.
typedef MessageBufferHandle_t SubscriberId;

// Represent a invalid subscriber id. All Subscriber identifiers greater than this are valid
static const SubscriberId INVALID_SUBSCRIBER_ID = NULL;

// no bigger messages are supported
static const size_t MAX_MESSAGE_SIZE = 32 + sizeof(EventId);

/**
 * Interface for Task.
 */
class TaskIfc : public EventSubscriberIfc {
public:

    /**
     * Destructor
     */
    virtual ~TaskIfc() {};

    /**
     * Gets the associated subscriber id
     * @return const SubscriberId 
     */
    virtual SubscriberId getSubscriberId() const = 0;

      /**
     * Gets the associated task id
     * @return const TaskId 
     */
    virtual TaskId getTaskId() const = 0;

    /**
     * Return true will shutdown this task after Run-to-Completion is finished.
     * 
     * @return true is shutdown task, no restart possible; otherwise false 
     */
    virtual bool isShutdownRequested() {
        return false;
    }

    /**
     * Initialize the task
     * 
     * @param taskId used to control wrapped task
     */
    virtual void initialize(const TaskId taskId) = 0;

    /**
     * Starts the task in a loop. Task can not be restartet when it's over!
     */
    virtual void start() = 0;
};


#endif // TASKIFC_H_
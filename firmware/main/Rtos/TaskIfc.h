#ifndef TASKIFC_H_
#define TASKIFC_H_

// Project includes
#include <HumiPlatform.h>

#include "Rtos/EventSubscriberIfc.h"
#include "Rtos/EventPublisherIfc.h"

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
     * Initialize the task
     */
    virtual void initialize() = 0;

    /**
     * Starts the task in a loop. Task can not be restartet when it's over!
     */
    virtual void start() = 0;
};


#endif // TASKIFC_H_
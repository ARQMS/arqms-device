#ifndef CONTROL_TASK_H_ 
#define CONTROL_TASK_H_

// Project includes
#include <HumiPlatform.h>
#include "Rtos/TaskBase.h"
#include "Rtos/EventSinglePublisher.h"

/**
 * 
 */
class ControlTask : public TaskBase {
public:
    EventSinglePublisher MeasSensor;

public:
    /**
     * Constructor
     */
    ControlTask();

    /**
     * Destructor
     */
    virtual ~ControlTask();
    
protected:
    /**
     * @see TaskBase::onInitialize()
     */
    virtual void onInitialize();

    /**
     * @see TaskBase::onStart()
     */
    virtual void onStart();

    /**
     * @see TaskBase::onExecute()
     */
    virtual void onExecute(EventId eventId, EventIfc* pEvent = NULL) override;


private:
    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    ControlTask(const ControlTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    ControlTask& operator=(const ControlTask& other);
};


#endif // CONTROL_TASK_H_
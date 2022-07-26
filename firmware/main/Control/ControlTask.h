#ifndef CONTROL_TASK_H_ 
#define CONTROL_TASK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/TimerEvent.h>

/**
 * This item is responsible for central logic and status of the device
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class ControlTask : public TaskBase<10, sizeof(int)> {
public:
    EventPublisherSingle GuiUpdater;

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
    virtual void onExecute(EventId eventId, Deserializer* pEvent = NULL) override;

private:
    // Helper methods
    void onHandleTestId(const TimerEvent& timer);

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    ControlTask(const ControlTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    ControlTask& operator=(const ControlTask& other);

    // Private Members
    float32_t m_currentQuality;
    TimerId m_testTimer;
};


#endif // CONTROL_TASK_H_
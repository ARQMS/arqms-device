#ifndef CONTROL_TASK_H_ 
#define CONTROL_TASK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/TimerEvent.h>

// Project includes
#include "Events/WifiStatusEvent.h"

/**
 * This item is responsible for central logic and status of the device
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class ControlTask : public TaskBase<5, sizeof(WifiStatusEvent)> {
public:
    EventPublisherSingle GuiUpdater;
    EventPublisherSingle CloudLink;

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
    virtual void onHandleEvent(EventId eventId, Deserializer* pEvent = NULL) override;
    virtual void onHandleTimer(const TimerId timerId) override;

private:
    // Helper methods
    void onHandleTestId();
    void onHandleWifiStatus(const WifiStatusEvent& status);

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

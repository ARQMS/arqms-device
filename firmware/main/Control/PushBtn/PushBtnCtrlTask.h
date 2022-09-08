#ifndef PUSH_BTN_CTRL_TASK_H_
#define PUSH_BTN_CTRL_TASK_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/TimerEvent.h>

// Project includes
#include <Events/ButtonEvent.h>


/**
 * This unit is called in the case of a push button event
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class PushBtnCtrlTask : public TaskBase<2, 10/*TODO sizeof(ButtonEvent)*/ >{
public:
    EventPublisherSingle Control;

public:
    /**
     * Constructor
     */
    PushBtnCtrlTask();

    /**
     * Destructor
     */
    ~PushBtnCtrlTask();

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
    virtual void onHandleTimer(TimerId timerId) override;

private:
    // Helper
    void onHandleButtonEvent(const ButtonEvent& event);
};


#endif // PUSH_BTN_CTRL_TASK_H_
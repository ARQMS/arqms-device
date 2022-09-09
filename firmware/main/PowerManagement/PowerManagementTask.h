#ifndef POWER_MANAGEMENT_TASK_H_
#define POWER_MANAMGEMENT_TASK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/EventPublisherMultiple.h>

// Project includes
#include "Events/BatteryStatusEvent.h"

/**
 * This unit reads the Batteries Level
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class PowerManagementTask : public TaskBase<1, sizeof(BatteryStatusEvent)>{

public: 
    EventPublisherSingle Control;
    /**
     * Constructor
     */
    PowerManagementTask();

    /**
     * Destructor
     */
    virtual ~PowerManagementTask();

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
    // Refresh Rate of Battery Level
    static const uint32_t REFRESH_RATE = 1000;
    // Helper Methods
    void onHandleBatteryLevel(const BatteryStatusEvent& quality);

    /**
     * Provide the private copy constructor so the compiler does not generate the default one
     */
    PowerManagementTask(const PowerManagementTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one
     */
    BatteryStatusEvent& operator=(const BatteryStatusEvent& other);

    // Private Members
    Timer* m_pRefreshTimer;
};

#endif // POWER_MANAGEMENT_TASK_H_

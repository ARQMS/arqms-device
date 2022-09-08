#ifndef CONTROL_TASK_H_ 
#define CONTROL_TASK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/TimerEvent.h>

// Project includes
#include "Events/WifiStatusEvent.h"
#include "Events/ButtonEvent.h"
#include "Events/BatteryStatusEvent.h"
#include "Control/Persistency/StorageDriverIfc.h"
#include "Control/CoreSM/CoreSM.h"

/**
 * This item is responsible for central logic and status of the device
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class ControlTask : public TaskBase<20, sizeof(WifiStatusEvent)> {
public:
    EventPublisherSingle Gui;
    EventPublisherSingle CloudLink;
    EventPublisherSingle MeasSensor;
    EventPublisherSingle MeasFilter;

public:
    /**
     * Constructor
     */
    ControlTask();

    /**
     * Destructor
     */
    virtual ~ControlTask();
    
    /**
     * @brief Set the Storage Driver object
     * 
     * @param pDriver 
     */
    static void setStorageDriver(StorageDriverIfc* const pDriver) {
        s_pNvsStorageDriver = pDriver;
    }

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
    void onHandleWifiStatus(const WifiStatusEvent& status);
    void onHandleBatteryStatus(const BatteryStatusEvent& status);
    void onHandleButton(const ButtonEvent& button);
    void sendDeviceStatus();

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    ControlTask(const ControlTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    ControlTask& operator=(const ControlTask& other);

    // Members
    static StorageDriverIfc* s_pNvsStorageDriver;
    
    Timer* m_pDelayTimer;
    CoreSM m_coreSm;

    WifiStatusEvent m_lastWifiStatus;
    BatteryStatusEvent m_lastBatteryStatus;

    bool m_wifiReceived;
    bool m_batteryReceived;
};


#endif // CONTROL_TASK_H_

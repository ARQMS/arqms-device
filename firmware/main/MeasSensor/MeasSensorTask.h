#ifndef MEAS_SENSOR_TASK_H_ 
#define MEAS_SENSOR_TASK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/TimerEvent.h>

// Project includes
#include "Events/WifiStatusEvent.h"
#include "Drivers/BME680Driver.h"

/**
 * This item is responsible for sensor measurements (opt. sensor calibration)
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class MeasSensorTask : public TaskBase<5, sizeof(WifiStatusEvent)> {
public:
    EventPublisherSingle Measurement;

public:
    /**
     * Constructor
     */
    MeasSensorTask();

    /**
     * Destructor
     */
    virtual ~MeasSensorTask();
    
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
    void onHandleSnapshot();
    void onHandleDataAvailable();

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    MeasSensorTask(const MeasSensorTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    MeasSensorTask& operator=(const MeasSensorTask& other);

    // Members
    BME680Driver m_bosch680Sensor;
    Timer* m_pWaitDataTimer;
};


#endif // MEAS_SENSOR_TASK_H_

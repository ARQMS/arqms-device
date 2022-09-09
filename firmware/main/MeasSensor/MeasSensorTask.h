#ifndef MEAS_SENSOR_TASK_H_ 
#define MEAS_SENSOR_TASK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/EventPublisherMultiple.h>
#include <HumiDevice.Rtos/TimerEvent.h>

// Project includes
#include "Drivers/BME680Driver.h"
#include "Drivers/ApplicationHardwareConfig.h"
#include "Events/DeviceInfoEvent.h"
#include "Events/SensorStatusEvent.h"

/**
 * This item is responsible for sensor measurements (opt. sensor calibration)
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class MeasSensorTask : public TaskBase<10, sizeof(EventId)> {
public:
    EventPublisherSingle Measurement;
    EventPublisherMultiple<2> Status;

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
    // Constants
    const static uint32_t READ_SENSOR_BURST_NO = SENSOR_AVG_FILTER_COUNT;

    // Helper methods
    void onHandleSnapshot();
    void onHandleDataAvailable();

    // start a new snapshot and increase burst index
    void startSnapshot();

    // sends a event with given status
    void sendStatus(const SensorStatus status);

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
    uint32_t m_burstIdx;
    bool_t m_initialized;
};


#endif // MEAS_SENSOR_TASK_H_

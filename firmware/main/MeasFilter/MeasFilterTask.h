#ifndef MEAS_FILTER_TASK_H_ 
#define MEAS_FILTER_TASK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherMultiple.h>
#include <HumiDevice.Rtos/TimerEvent.h>

// Project includes
#include "Drivers/ApplicationHardwareConfig.h"
#include "Events/SensorDataEvent.h"
#include "MovingAvgFilter.h"

/**
 * This unit filters the measurements.
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class MeasFilterTask : public TaskBase<5, sizeof(SensorDataEvent)> {
public:
    EventPublisherMultiple<2> Measurement;

public:
    /**
     * Constructor
     */
    MeasFilterTask();

    /**
     * Destructor
     */
    virtual ~MeasFilterTask();
    
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
    void onHandleSensorData(SensorDataEvent& data);
    void publish();

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    MeasFilterTask(const MeasFilterTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    MeasFilterTask& operator=(const MeasFilterTask& other);

    // members
    MovingAvgFilter m_tempFilter;
    MovingAvgFilter m_pressureFilter;
    MovingAvgFilter m_gasResistanceFilter;
    MovingAvgFilter m_humidtyFilter;
};


#endif // MEAS_FILTER_TASK_H_

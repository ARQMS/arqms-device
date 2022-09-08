#ifndef GUI_UPDATER_H_ 
#define GUI_UPDATER_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>

// Project includes
#include "AirIndicatorDriver.h"
#include "Events/AirQualityEvent.h"
#include "Events/WifiStatusEvent.h"
#include "Events/SensorDataEvent.h"
#include "Events/SensorStatusEvent.h"

#include "Drivers/SK6805Driver.h"

/**
 * This unit updates all user interface components
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 * @see https://github.com/ARQMS/arqms-device/wiki/Mechanics#hmi-interface
 */
class GuiUpdaterTask : public TaskBase<10, sizeof(AirQualityEvent)> {
public:
    // no outputs

public:
    /**
     * Constructor
     */
    GuiUpdaterTask();

    /**
     * Destructor
     */
    virtual ~GuiUpdaterTask();
    
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
    // refresh rate of ui
    static const uint32_t REFRESH_RATE = (1 / 20.f) * 1000; // 50ms

    // Helper methods
    void onHandleAirQuality(const AirQualityEvent& quality);
    void onHandleWifiStatus(const WifiStatusEvent& wifiStatus);
    void onHandleSensorData(const SensorDataEvent& data);
    void onHandleSensorStatus(const SensorStatusEvent& status);
    void onHandleRefresh();

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    GuiUpdaterTask(const GuiUpdaterTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    GuiUpdaterTask& operator=(const GuiUpdaterTask& other);

    // Private Members
    AirIndicatorDriver m_airIndicator;
    SK6805Driver m_ctrlIndicator;

    Timer* m_pRefreshTimer;
};


#endif // GUI_UPDATER_H_
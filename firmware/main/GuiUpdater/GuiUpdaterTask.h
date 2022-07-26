#ifndef GUI_UPDATER_H_ 
#define GUI_UPDATER_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>

// Project includes
#include "AirIndicatorDriver.h"
#include "Events/AirQualityEvent.h"

/**
 * This unit updates all user interface components
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class GuiUpdaterTask : public TaskBase<2, sizeof(AirQualityEvent)> {
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

private:

    // Helper methods
    void onHandleAirQuality(const AirQualityEvent& quality);

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
};


#endif // GUI_UPDATER_H_
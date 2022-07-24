#ifndef CLOUD_LINK_H_ 
#define CLOUD_LINK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>

// Project includes
#include "Events/WifiSettingsEvent.h"

/**
 * This item is responsible for connection to cloud
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class CloudLinkTask : public TaskBase<5, sizeof(WifiSettingsEvent)> {
public:
    EventPublisherSingle Control;

public:
    /**
     * Constructor
     */
    CloudLinkTask();

    /**
     * Destructor
     */
    virtual ~CloudLinkTask();
    
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
    void onHandleWifiSettings(const WifiSettingsEvent& settings);

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    CloudLinkTask(const CloudLinkTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    CloudLinkTask& operator=(const CloudLinkTask& other);

    // Private Members
};


#endif // CLOUD_LINK_H_
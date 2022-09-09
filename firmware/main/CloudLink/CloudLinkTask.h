#ifndef CLOUD_LINK_H_ 
#define CLOUD_LINK_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/TaskBase.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>
#include <HumiDevice.Rtos/EventPublisherMultiple.h>

// Project includes
#include "Events/WifiSettingsEvent.h"
#include "Events/WifiStatusEvent.h"
#include "Events/SensorDataEvent.h"
#include "Events/DeviceStatusEvent.h"
#include "Wifi/Wifi.h"
#include "CloudLinkSenderIfc.h"
#include "ConfigurationService/ConfigurationService.h"
#include "MqttService/MqttService.h"

/**
 * This item is responsible for connection to cloud
 * 
 * @see https://github.com/ARQMS/arqms-device/wiki/Firmware#decomposition
 */
class CloudLinkTask : public TaskBase<10, sizeof(DeviceInfoEvent)>, CloudLinkSenderIfc {
public:
    EventPublisherMultiple<2> Status;
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
    
    /**
     * @see CloudLinkSenderIfc::sendWifiStatus
     */
    virtual void sendWifiStatus(const WifiStatus status, int32_t rssi = 0) override;

    /**
     * @see CloudLinkSenderIfc::onDeviceConfig
     */
    virtual void onDeviceConfig(const HDPDeviceConfig& msg) override;

    /**
     * @see CloudLinkSenderIfc::onUpdateInfo
     */
    virtual void onUpdateInfo(const HDPUpdateInfo& msg) override;

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
    // constant
    static const uint32_t TIMEOUT_SERVICE_MODE_MS = 60 * 1000; // 1min

    // Helper methods
    void onHandleWifiSettings(const WifiSettingsEvent& settings);
    void onHandleDeviceSettings(const DeviceInfoEvent& settings);
    void onHandleDeviceInfo(const DeviceStatusEvent& status);
    void onHandleSensorDataEvent(const SensorDataEvent& settings);
    void onHandleTimeout();
    void onHandleWifiStateUpdate();

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    CloudLinkTask(const CloudLinkTask& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    CloudLinkTask& operator=(const CloudLinkTask& other);

    // Private Members
    ConfigurationService m_ctrlHandler;
    MqttService m_mqttService;
    Wifi m_wifi;
    WifiStatusEvent m_lastWifiEvent;

    Timer* m_pTimeoutTimer;
};


#endif // CLOUD_LINK_H_
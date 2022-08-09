#ifndef WIFI_H_
#define WIFI_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// IDF includes
#include "esp_wifi.h"

// Project includes
#include "ConfigurationService/ConfigurationServiceIfc.h"
#include "WifiStateMachine.h"
#include "WifiStateMachineCallbackIfc.h"
#include "MqttService/MqttServiceIfc.h"
#include "Events/WifiSettingsEvent.h"
#include "Events/DeviceSettingsEvent.h"

/**
 * Represents the wifi phy
 */
class Wifi {
public:
    /**
     * Constructor
     */
    Wifi(WifiStateMachineCallbackIfc& wifiCallback, ConfigurationServiceIfc& ctrlHandler, MqttServiceIfc& mqttService);

    /**
     * Destructor
     */
    ~Wifi();

    /**
     * Initialize wifi peripheral
     */
    void initialize();

    /**
     * Start service mode
     */
    void startServiceMode();

    /**
     * Normal mode
     */
    void startNormalMode();

    /**
     * Updates internal wifi structure
     * @param settings to apply
     */
    void updateWifiSettings(const WifiSettingsEvent& settings);

    /**
     * Updates internal device structure
     * @param settings to apply
     */
    void updateDeviceSettings(const DeviceSettingsEvent& settings);

    /**
     * Called after timout is reached.
     */
    void onTimeout();

    /**
     * Resets the wifi peripheral
     */
    void reset();
private:
    // Constants
    const static uint8_t MAXIMUM_RETRY = 5U;            //< maximum wifi re-try

    // Helper methods
    void onClientConnected();
    void onClientDisconnected();
    void onApStarted();
    void onStaStarted();
    void onWifiConnected();
    void onWifiDisconnected();
    void checkEspError(const esp_err_t status);

    // Idf callback
    static void onWifiEventHandler(void* param, esp_event_base_t eventBase, int32_t eventId, void* eventData);

    bool m_wifiSettingsUpdated;
    bool m_deviceSettingsUpdated;

    WifiSettingsEvent m_wifiSettings;
    DeviceSettingsEvent m_deviceSettings;

    WifiStateMachine m_wifiSm;
    WifiStateMachineCallbackIfc& m_sender;
    uint32_t m_retryCounter;

    ConfigurationServiceIfc& m_ctrlHandler;
    MqttServiceIfc& m_mqttService;
};

#endif // WIFI_H_
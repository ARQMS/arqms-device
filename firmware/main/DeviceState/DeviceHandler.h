#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "Platform.h"
#include "WiFi/WiFiManager.h"
#include "DeviceStorage/DeviceStorage.h"
#include "Sensor/SensorManager.h"

/**
 * A handler for state machine
 */
class DeviceHandler {
public:
    /**
     * Constructor
     */
    explicit DeviceHandler(WiFiManager& wifi, SensorManager& sensor, DeviceStorage& storage);

    /**
     * Deconstructor
     */
    ~DeviceHandler() { }

    /**
     * Checks the stored wifi configuration
     * 
     * @return bool true when wifi configuration is valid; otherwise false
     */
    bool isWifiCfgValid() const;

    /**
     * Checks if all required task are done in current run.
     * 
     * @return bool true when all task are done; otherwise false
     */
    bool isAllTaskDone() const;

    /**
     * Checks if new firmware version is available to download and install
     * 
     * @return bool true when available; otherwise false
     */
    bool isNewVersionAvailable() const;

    /**
     * checks if firmware update was successfull installed
     * 
     * @return bool true when installed, otherwise false
     */
    bool isFirmwareUpdateFailed() const;

    /**
     * Checks if all required data published.
     * 
     * @return bool true when all tasks are done, otherwise false
     */
    bool isDataPublished() const;

    /**
     * Checks if any clients are connected in AP mode.
     * 
     * @return true if at least one client is connected; otherwise false
     */
    bool anyClientConnected() const;

    /**
     * Get the current Wifi State 
     * 
     * @return WiFiState 
     */
    WiFiState getWifiState() const;

    /**
     * Connects to the configured wifi. 
     */
    void connectToWiFi();

    /**
     * starts the Access Point mode for service
     */
    void enableAPMode();

    /**
     * Starts the firmware update 
     */
    void startFirmwareUpdate();

    /**
     * Start sensor data acquisition. 
     */
    void startDataAcquisition();

    /**
     * Enter sleep mode
     */
    void enterSleep();
private:
    /**
     * Private copy constructor.
     */
    DeviceHandler(const DeviceHandler& other);

    /**
     * Private assignment operator.
     */
    DeviceHandler& operator=(const DeviceHandler& other);

    // Members
    WiFiManager& wifiManager;
    SensorManager& sensor;
    DeviceStorage& storage;

    // internal states
    bool updateInstallationFailed;
    bool deviceStatePublished;
    bool sensorDataPublished;
};

#endif // DEVICEHANDLER_H
#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "Platform.h"
#include "WiFi/WiFiManager.h"
#include "DeviceStorage/DeviceStorage.h"

/**
 * A handler for state machine
 */
class DeviceHandler {
public:
    /**
     * Constructor
     */
    explicit DeviceHandler(WiFiManager& wifiManager, DeviceStorage& storage);

    /**
     * Deconstructor
     */
    ~DeviceHandler() { }

    /**
     * Checks the stored wifi configuration
     * 
     * @return bool true when wifi configuration is valid; otherwise false
     */
    bool isWifiCfgValid();

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
    DeviceStorage& storage;
};

#endif // DEVICEHANDLER_H
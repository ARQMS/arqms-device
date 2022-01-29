#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "Platform.h"
#include "DeviceStorage/DeviceStorage.h"

/**
 * Represents the Wifi Management for HumiDevice
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/network/esp_wifi.html
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-guides/wifi.html
 */
class WiFiManager {
public:
    /**
     * Construct a new WifiManager object
     */
    explicit WiFiManager(DeviceStorage& storage);

    /**
     * Destroy the WifiManager object
     */
    ~WiFiManager();

    /**
     * Initialize WiFi Allocate resource for WiFi driver, such as WiFi control structure,
     * RX/TX buffer, WiFi NVS structure etc. This WiFi also starts WiFi task. 
     * 
     * @note ensure NVS is already initialized!
     */
    void initialize();

    /**
     * Checks wifi station mode (STA) configuration. It's invalid when any mendetory 
     * field is missing (eg factory reset, or NVS error)
     * 
     * @return bool true if STA configuration is valid; otherwise false
     */
    bool isSTACfgValid();
private:
    /**
     * Construct a new WifiManager object
     * 
     * @param copy instance
     */
    WiFiManager(const WiFiManager& copy);

    // Members
    DeviceStorage& storage;
};

#endif // WIFI_MANAGER_H
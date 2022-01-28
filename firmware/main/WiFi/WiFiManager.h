#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "Platform.h"

/**
 * @brief Represents the Wifi Management for HumiDevice
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/network/esp_wifi.html
 */
class WiFiManager {
public:
    /**
     * @brief Construct a new WifiManager object
     */
    WiFiManager();

    /**
     * @brief Destroy the WifiManager object
     */
    ~WiFiManager();

    /**
     * @brief Initialize WiFi Allocate resource for WiFi driver, such as WiFi control structure,
     * RX/TX buffer, WiFi NVS structure etc. This WiFi also starts WiFi task. 
     * 
     * @note ensure NVS is already initialized!
     */
    void initialize();

private:
    /**
     * @brief Construct a new WifiManager object
     * 
     * @param copy instance
     */
    WiFiManager(const WiFiManager& copy);
};

#endif // WIFI_MANAGER_H
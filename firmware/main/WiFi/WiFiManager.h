#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "Platform.h"
#include "DeviceStorage/DeviceStorage.h"

/**
 * Represents the wifi states 
 */
enum class WiFiState {
    DISABLED,
    CONNECTED,
    FAILED,
    DISCONNECTED,
};

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
    bool isWifiCfgValid() const;

    /**
     * Get the Wifi State
     * 
     * @return WiFiState 
     */
    WiFiState getWifiState() const;

    /**
     * gets the number of connected clients. It's only useful number when
     * device is in AP mode.
     * 
     * @return uint32_t number of connected clients.
     */
    uint32_t countClients();
private:
    /**
     * Construct a new WifiManager object
     * 
     * @param copy instance
     */
    WiFiManager(const WiFiManager& copy);

    // Members
    DeviceStorage& storage;
    WiFiState wifiState;
};

#endif // WIFI_MANAGER_H
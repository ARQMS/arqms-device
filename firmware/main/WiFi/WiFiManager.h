#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "Platform.h"
#include "DeviceStorage/DeviceStorage.h"
#include "Events/EventLoopIfc.h"
#include "Events/EventHandlerIfc.h"

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
class WiFiManager : public EventHandlerIfc {
public:
    /**
     * Construct a new WifiManager object
     */
    explicit WiFiManager(DeviceStorage& storage, EventLoopIfc& eventLoop);

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

    /**
     * Starts the wifi peripheral in STA mode
     * 
     * @param cfg to use to connect
     */
    void startSTA(WifiParameters& cfg);

    /**
     * Starts the wifi peripheral as access point. Clients can connect to this device
     * 
     * @see countClients()
     */
    void startAP(WifiParameters& cfg);

    /**
     * @see EventHandlerIfc
     */
    void onEvent(esp_event_base_t base, int32_t id, void* data) override;

private:
    // the maximum retry connection
    static const uint32_t MAXIMUM_RECONNECTING_RETRY = 5;

    /**
     * Construct a new WifiManager object
     * 
     * @param copy instance
     */
    WiFiManager(const WiFiManager& copy);

    // Members
    DeviceStorage& storage;
    EventLoopIfc& eventLoop;
    WiFiState wifiState;

    uint32_t retryNum;
    uint32_t apClients;
};

#endif // WIFI_MANAGER_H
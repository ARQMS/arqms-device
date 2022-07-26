#ifndef WIFI_STATUS_EVENT_H_
#define WIFI_STATUS_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * Represents the wifi status
 */
enum class WifiStatus : uint8_t {
    // STA Mode
    DISCONNECTED,
    CONNECTED,
    LOST,
    CONNECTING,

    // AP mode
    CLIENT_SEARCHING,
    CLIENT_CONNECTED,
    CLIENT_DISCONNECTED,
    CLIENT_TIMEOUT,

    // common
    DISABLED,
    UNKNOWN_ERROR
};

/**
 * A wifi status event which holds information about current wifi state
 */
class WifiStatusEvent : public EventIfc {
public:
    /**
     * Constructor
     */
    explicit WifiStatusEvent();
    WifiStatusEvent(Deserializer& deserialize);

    /**
     * Destructor
     */
    virtual ~WifiStatusEvent();

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer) override;

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const override;

    // Getter & Setter
    void setRssi(const int32_t value);
    int32_t getRssi() const;

    void setWifiStatus(const WifiStatus value);
    WifiStatus getWifiStatus() const;
    
private:
    int32_t m_rssi; 
    WifiStatus m_wifiStatus;
};



#endif // WIFI_STATUS_EVENT_H_
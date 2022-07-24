#ifndef AIR_QUALITY_EVENT_H_
#define AIR_QUALITY_EVENT_H_

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

    // AP mode
    CLIENT_SEARCHING,
    CLIENT_CONNECTED,
    CLIENT_DISCONNECTED,

    // common
    UNKNOWN_ERROR
};

/**
 * An air quality event which holds all information about air quality
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
    void setRssi(const int32_t value) { m_rssi = value; }
    int32_t getRssi() const { return m_rssi; }

    void setWifiStatus(const WifiStatus value) { m_wifiStatus = value; }
    WifiStatus getWifiStatus() const { return m_wifiStatus; }
private:
    int32_t m_rssi; 
    WifiStatus m_wifiStatus;
};



#endif // AIR_QUALITY_EVENT_H_
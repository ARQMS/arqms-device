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

    // Protocol
    MQTT_CONNECTED,
    MQTT_DISCONNECTED,
    MQTT_RECEIVED,
    MQTT_SENDING,
    MQTT_SENDED,

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
     * Default constructor.
     */
    WifiStatusEvent(void);

    /**
     * Custom constructor.
     */
    WifiStatusEvent(const int32_t rssi, const WifiStatus status);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    WifiStatusEvent(const WifiStatusEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit WifiStatusEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~WifiStatusEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    WifiStatusEvent& operator=(const WifiStatusEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const WifiStatusEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const WifiStatusEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for rssi
     */
    void setRssi(const int32_t rssi);

    /**
     * Getter for rssi
     */
    int32_t getRssi(void) const;

    /**
     * Setter for status
     */
    void setStatus(const WifiStatus status);

    /**
     * Getter for status
     */
    WifiStatus getStatus(void) const;
    
private:
    int32_t m_rssi; 
    WifiStatus m_status;
};



#endif // WIFI_STATUS_EVENT_H_
#ifndef DEVICE_STATUS_EVENT_H_
#define DEVICE_STATUS_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

#include "BatteryStatusEvent.h"
#include "WifiStatusEvent.h"

/**
 * Device status info event
 */
class DeviceStatusEvent : public EventIfc {
public:
    /**
     * Default Constructor
     */
    DeviceStatusEvent(void);

    /**
     * Custom Constructor
     */
    DeviceStatusEvent(BatteryStatusEvent& batteryStatus, WifiStatusEvent& wifiStatus, uint32_t uptime);

    /**
     * Copy Constructor
     * @param other The copied instance
     */
    DeviceStatusEvent(const DeviceStatusEvent& other);

    /**
     * Constructor from deserializer
     */
    explicit DeviceStatusEvent(Deserializer& deserializer);

    /**
     * Destructor
     */
    virtual ~DeviceStatusEvent(void);

    /**
     * Assignment operator
     * @param other the assignator instance
     */
    DeviceStatusEvent& operator=(const DeviceStatusEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const DeviceStatusEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const DeviceStatusEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for Battery Status
     */
    void setBatteryStatus(const BatteryStatusEvent& status);

    /**
     * Getter for Battery Status
     */
    const BatteryStatusEvent& getBatteryStatus(void) const;

    /**
     * Setter for Wifi Status
     */
    void setWifiStatus(const WifiStatusEvent& status);

    /**
     * Getter for Wifi Status
     */
    const WifiStatusEvent& getWifiStatus(void) const;

    /**
     * Setter for uptime
     */
    void setUptime(const uint32_t uptime);

    /**
     * Getter for uptime
     */
    uint32_t getUptime(void) const;

private:
    BatteryStatusEvent m_batteryStatus;
    WifiStatusEvent m_wifiStatus;
    uint32_t m_uptime;
};

#endif // DEVICE_STATUS_EVENT_H_
#ifndef DEVICE_INFO_EVENT_H_
#define DEVICE_INFO_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

#include "BatteryStatusEvent.h"
#include "WifiStatusEvent.h"

/**
 * Device info event
 */
class DeviceInfoEvent : public EventIfc {
public:
    /**
     * Default Constructor
     */
    DeviceInfoEvent(void);

    /**
     * Custom Constructor
     */
    DeviceInfoEvent(BatteryStatusEvent& batteryStatus, WifiStatusEvent& wifiStatus, uint32_t uptime);

    /**
     * Copy Constructor
     * @param other The copied instance
     */
    DeviceInfoEvent(const DeviceInfoEvent& other);

    /**
     * Constructor from deserializer
     */
    explicit DeviceInfoEvent(Deserializer& deserializer);

    /**
     * Destructor
     */
    virtual ~DeviceInfoEvent(void);

    /**
     * Assignment operator
     * @param other the assignator instance
     */
    DeviceInfoEvent& operator=(const DeviceInfoEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const DeviceInfoEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const DeviceInfoEvent& right) const;

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

#endif
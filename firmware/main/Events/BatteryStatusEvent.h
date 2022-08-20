#ifndef BATTERY_STATUS_EVENT_H_
#define BATTERY_STATUS_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

class BatteryStatusEvent : public EventIfc {
public:
    // Constants

    /**
     * Default Constructor
     */
    BatteryStatusEvent(void);

    /**
     * Custom Constructor
     */
    BatteryStatusEvent(uint8_t level);

    /**
     * Copy Constructor
     * @param other The copied instance
     */
    BatteryStatusEvent(const BatteryStatusEvent& other);

    /**
     * Constructor from deserializer
     */
    explicit BatteryStatusEvent(Deserializer& deserializer);

    /**
     * Destructor
     */
    virtual ~BatteryStatusEvent(void);

    /**
     * Assignment operator
     * @param other the assignator instance
     */
    BatteryStatusEvent& operator=(const BatteryStatusEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const BatteryStatusEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const BatteryStatusEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for Battery Level
     */
    void setLevel(uint8_t level);

    /**
     * Getter for Battery Level
     */
    uint8_t getLevel(void);

private:
    uint8_t m_level;
};

#endif
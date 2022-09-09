#ifndef DEVICE_CONFIG_EVENT_H_
#define DEVICE_CONFIG_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * A device settings event which holds configuration for device
 */
class DeviceConfigEvent : public EventIfc {
public:
    // Constants
    static const size_t MAX_CHANNEL_LENGTH  = 32;

    /**
     * Default constructor.
     */
    DeviceConfigEvent(void);

    /**
     * Custom constructor.
     */
    DeviceConfigEvent(const char8_t channel[MAX_CHANNEL_LENGTH], const uint32_t interval);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    DeviceConfigEvent(const DeviceConfigEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit DeviceConfigEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~DeviceConfigEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    DeviceConfigEvent& operator=(const DeviceConfigEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const DeviceConfigEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const DeviceConfigEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for channel
     */
    void setChannel(const char8_t channel[MAX_CHANNEL_LENGTH]);

    /**
     * Getter for channel
     */
    void getChannel(char8_t channel[MAX_CHANNEL_LENGTH]) const;

    /**
     * Setter for interval
     */
    void setInterval(const uint32_t interval);

    /**
     * Getter for interval
     */
    uint32_t getInterval() const;

private:
    char8_t m_channel[MAX_CHANNEL_LENGTH];
    uint32_t m_interval;
};



#endif // DEVICE_CONFIG_EVENT_H_
#ifndef DEVICE_SETTING_EVENT_H_
#define DEVICE_SETTING_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * A device settings event which holds configuration for device
 */
class DeviceSettingsEvent : public EventIfc {
public:
    // Constants
    static const size_t MAX_SN_LENGTH  = 32;
    static const size_t MAX_ROOM_LENGTH  = 32;
    static const size_t MAX_BROKER_URI_LENGTH  = 64;

    /**
     * Default constructor.
     */
    DeviceSettingsEvent(void);

    /**
     * Custom constructor.
     */
    DeviceSettingsEvent(const char8_t brokerUri[MAX_BROKER_URI_LENGTH], const char8_t sn[MAX_SN_LENGTH], const char8_t room[MAX_ROOM_LENGTH], const uint32_t interval);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    DeviceSettingsEvent(const DeviceSettingsEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit DeviceSettingsEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~DeviceSettingsEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    DeviceSettingsEvent& operator=(const DeviceSettingsEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const DeviceSettingsEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const DeviceSettingsEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for brokerUri
     */
    void setBrokerUri(const char8_t brokerUri[MAX_BROKER_URI_LENGTH]);

    /**
     * Getter for brokerUri
     */
    void getBrokerUri(char8_t brokerUri[MAX_BROKER_URI_LENGTH]) const;

    /**
     * Setter for sn
     */
    void setSn(const char8_t sn[MAX_SN_LENGTH]);

    /**
     * Getter for sn
     */
    void getSn(char8_t sn[MAX_SN_LENGTH]) const;

    /**
     * Setter for room
     */
    void setRoom(const char8_t room[MAX_ROOM_LENGTH]);

    /**
     * Getter for room
     */
    void getRoom(char8_t room[MAX_ROOM_LENGTH]) const;

    /**
     * Setter for interval
     */
    void setInterval(const uint32_t interval);

    /**
     * Getter for interval
     */
    uint32_t getInterval() const;

private:
    char8_t m_sn[MAX_SN_LENGTH];
    char8_t m_room[MAX_ROOM_LENGTH];
    char8_t m_brokerUri[MAX_BROKER_URI_LENGTH];
    uint32_t m_interval;
};



#endif // DEVICE_SETTING_EVENT_H_
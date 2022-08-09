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
    static const size_t MAX_BROKER_URI_LENGTH  = 64;

    /**
     * Constructor
     */
    explicit DeviceSettingsEvent();
    DeviceSettingsEvent(Deserializer& deserialize);

    /**
     * Destructor
     */
    virtual ~DeviceSettingsEvent();

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer) override;

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const override;

    void setSn(const char8_t password[MAX_SN_LENGTH]);
    void getSn(char8_t password[MAX_SN_LENGTH]) const;

    void setBrokerUri(const char8_t brokerUri[MAX_BROKER_URI_LENGTH]);
    void getBrokerUri(char8_t brokerUri[MAX_BROKER_URI_LENGTH]) const;

private:
    char8_t m_Sn[MAX_SN_LENGTH];
    char8_t m_brokerUri[MAX_BROKER_URI_LENGTH];
};



#endif // DEVICE_SETTING_EVENT_H_
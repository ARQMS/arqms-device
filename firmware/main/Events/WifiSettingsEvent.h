#ifndef WIFI_SETTINGS_EVENT_H_
#define WIFI_SETTINGS_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * List of supported wifi modes
 */
enum class WifiMode : uint8_t {
    NONE,
    STA,
    AP
};

/**
 * A wifi settings event which holds configuration for wifi
 */
class WifiSettingsEvent : public EventIfc {
public:
    // Constants
    // according to IEEE Std 802.11-2007 the maximal length of SSID and PASSWORD are given
    static const size_t MAX_SSID_LENGTH = 32;
    static const size_t MAX_PWD_LENGTH  = 64;

    /**
     * Constructor
     */
    explicit WifiSettingsEvent();
    WifiSettingsEvent(Deserializer& deserialize);

    /**
     * Destructor
     */
    virtual ~WifiSettingsEvent();

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer) override;

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const override;

    // Getter & Setter
    void setMode(const WifiMode mode);
    WifiMode getMode() const;

    void setSSID(const char8_t ssid[MAX_SSID_LENGTH]);
    void getSSID(char8_t ssid[MAX_SSID_LENGTH]) const;

    void setPWD(const char8_t password[MAX_PWD_LENGTH]);
    void getPWD(char8_t password[MAX_PWD_LENGTH]) const;

private:
    WifiMode m_mode;
    char8_t m_ssid[MAX_SSID_LENGTH];
    char8_t m_pwd[MAX_PWD_LENGTH];
};



#endif // WIFI_SETTINGS_EVENT_H_
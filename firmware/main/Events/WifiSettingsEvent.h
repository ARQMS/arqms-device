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
     * Default constructor.
     */
    WifiSettingsEvent(void);

    /**
     * Custom constructor.
     */
    WifiSettingsEvent(const WifiMode mode, const char8_t ssid[MAX_SSID_LENGTH], const char8_t password[MAX_PWD_LENGTH]);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    WifiSettingsEvent(const WifiSettingsEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit WifiSettingsEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~WifiSettingsEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    WifiSettingsEvent& operator=(const WifiSettingsEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const WifiSettingsEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const WifiSettingsEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for mode
     */
    void setMode(const WifiMode mode);

    /**
     * Getter for mode
     */
    WifiMode getMode(void) const;

    /**
     * Setter for ssid
     */
    void setSsid(const char8_t ssid[MAX_SSID_LENGTH]);

    /**
     * Getter for ssid
     */
    void getSsid(char8_t ssid[MAX_SSID_LENGTH]) const;

    /**
     * Setter for password
     */
    void setPassword(const char8_t password[MAX_PWD_LENGTH]);

    /**
     * Getter for password
     */
    void getPassword(char8_t password[MAX_PWD_LENGTH]) const;
private:
    WifiMode m_mode;
    char8_t m_ssid[MAX_SSID_LENGTH];
    char8_t m_password[MAX_PWD_LENGTH];
};



#endif // WIFI_SETTINGS_EVENT_H_
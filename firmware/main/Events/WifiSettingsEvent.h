#ifndef AIR_QUALITY_EVENT_H_
#define AIR_QUALITY_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * List of supported wifi modes
 */
enum class WifiMode : uint8_t {
    NONE,
    STL,
    AP
};

/**
 * An air quality event which holds all information about air quality
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
    void setMode(const WifiMode mode) { m_mode = mode; }
    WifiMode getMode() const { return m_mode; }

    void setSSID(const char8_t ssid[MAX_SSID_LENGTH]) {       
        if (ssid != NULL) {
            mempcpy(&m_ssid[0], &ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
        } 
    }

    void getSSID(char8_t ssid[MAX_SSID_LENGTH]) const { 
        if (ssid != NULL) {
            mempcpy(&ssid[0], &m_ssid[0], MAX_SSID_LENGTH * sizeof(char8_t));
        }
    }

    void setPWD(const char8_t password[MAX_PWD_LENGTH]) {       
        if (password != NULL) {
            mempcpy(&m_pwd[0], &password[0], MAX_PWD_LENGTH * sizeof(char8_t));
        } 
    }
    
    void getPWD(char8_t password[MAX_PWD_LENGTH]) const { 
        if (password != NULL) {
            mempcpy(&password[0], &m_pwd[0], MAX_PWD_LENGTH * sizeof(char8_t));
        }
    }

private:
    WifiMode m_mode;
    char8_t m_ssid[MAX_SSID_LENGTH];
    char8_t m_pwd[MAX_PWD_LENGTH];
};



#endif // AIR_QUALITY_EVENT_H_
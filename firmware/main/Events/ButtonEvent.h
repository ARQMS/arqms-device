#ifndef BUTTON_EVENT_H_
#define BUTTON_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

#include <Drivers/ApplicationHardwareConfig.h>


/**
 * Represents the button status
 */
enum class ButtonStatus : uint8_t {
    SHORT_PRESS,
    LONG_PRESS,
};

/**
 * Represents the button id
 */
enum class ButtonId : uint8_t {
    USER = (int8_t)USR_BTN,
    SW_RESET = (int8_t)PRG_SW_RST_BTN,
};


/**
 * A button event
 */
class ButtonEvent : public EventIfc {
public:
    
    /**
     * Default constructor.
     */
    ButtonEvent(void);

    /**
     * Custom constructor.
     */
    ButtonEvent(const ButtonId id, const ButtonStatus status);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    ButtonEvent(const ButtonEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit ButtonEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~ButtonEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    ButtonEvent& operator=(const ButtonEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const ButtonEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const ButtonEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for button id
     */
    void setButtonId(const ButtonId rssi);

    /**
     * Getter for button id
     */
    ButtonId getButtonId(void) const;

    /**
     * Setter for status
     */
    void setStatus(const ButtonStatus status);

    /**
     * Getter for status
     */
    ButtonStatus getStatus(void) const;
    
private:
    ButtonId m_id; 
    ButtonStatus m_status;
};



#endif // BUTTON_EVENT_H_
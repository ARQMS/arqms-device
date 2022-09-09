#ifndef DISPLAY_INFO_EVENT_H_
#define DISPLAY_INFO_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * A display info event
 */
class DisplayInfoEvent : public EventIfc {
public:
    
    /**
     * Default constructor.
     */
    DisplayInfoEvent(void);

    /**
     * Custom constructor.
     */
    DisplayInfoEvent(const bool isActive);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    DisplayInfoEvent(const DisplayInfoEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit DisplayInfoEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~DisplayInfoEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    DisplayInfoEvent& operator=(const DisplayInfoEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const DisplayInfoEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const DisplayInfoEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for status
     */
    void setStatus(const bool status);

    /**
     * Getter for status
     */
    bool getStatus(void) const;
    
private:
    bool m_status;
};



#endif // DISPLAY_INFO_EVENT_H_
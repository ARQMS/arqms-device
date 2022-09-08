#ifndef SENSOR_STATUS_EVENT_H_
#define SENSOR_STATUS_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * Represents the sensor status
 */
enum class SensorStatus : uint8_t {
    IDLE,
    ACQUIRE,
    ERROR
};

/**
 * A sensor status event which holds information about current sensor state
 */
class SensorStatusEvent : public EventIfc {
public:
    
    /**
     * Default constructor.
     */
    SensorStatusEvent(void);

    /**
     * Custom constructor.
     */
    SensorStatusEvent(const SensorStatus status);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    SensorStatusEvent(const SensorStatusEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit SensorStatusEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~SensorStatusEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    SensorStatusEvent& operator=(const SensorStatusEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const SensorStatusEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const SensorStatusEvent& right) const;

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
    void setStatus(const SensorStatus status);

    /**
     * Getter for status
     */
    SensorStatus getStatus(void) const;
    
private:
    SensorStatus m_status;
};



#endif // SENSOR_STATUS_EVENT_H_
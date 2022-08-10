#ifndef AIR_QUALITY_EVENT_H_
#define AIR_QUALITY_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * An air quality event which holds all information about air quality
 */
class AirQualityEvent : public EventIfc {
public:

    /**
     * Default constructor.
     */
    AirQualityEvent(void);

    /**
     * Custom constructor.
     */
    explicit AirQualityEvent(const float32_t Quality);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    AirQualityEvent(const AirQualityEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit AirQualityEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~AirQualityEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    AirQualityEvent& operator=(const AirQualityEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const AirQualityEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const AirQualityEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for Quality
     */
    void setQuality(const float32_t Quality);

    /**
     * Getter for Quality
     */
    float32_t getQuality(void) const;
private:
    // range 0..1
    // 0.0 means bad quality
    // 1.0 means best quality
    float32_t m_quality; 
};



#endif // AIR_QUALITY_EVENT_H_
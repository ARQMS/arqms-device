#ifndef AIR_QUALITY_EVENT_H_
#define AIR_QUALITY_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>
#include <HumiDevice.Serialization/Deserializer.h>

/**
 * Enum for air quality 
 */
enum AirQuality {
    UNKNOWN = 0,
    GOOD    = 1,
    MOD     = 2,
    POOR    = 3,
};

class AirQualityEvent : public EventIfc {
public:
    /**
     * Constructor
     */
    explicit AirQualityEvent();
    AirQualityEvent(Deserializer& deserialize);

    /**
     * Destructor
     */
    virtual ~AirQualityEvent();

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer) override;

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const override;

    // Getter & Setter
    void setQuality(const AirQuality quality) { m_quality = quality; }
    AirQuality getQuality() const { return m_quality; }

private:
    AirQuality m_quality;
};



#endif // AIR_QUALITY_EVENT_H_
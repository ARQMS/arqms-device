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
    void setQuality(const float32_t quality);
    float32_t getQuality() const;

private:
    // range 0..1
    // 0.0 means bad quality
    // 1.0 means best quality
    float32_t m_quality; 
};



#endif // AIR_QUALITY_EVENT_H_
#ifndef SENSOR_DATA_EVENT_H_
#define SENSOR_DATA_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>

/**
 * Represents the filtered sensor data
 */
class SensorDataEvent : public EventIfc {
public:
    
    /**
     * Default constructor.
     */
    SensorDataEvent(void);

    /**
     * Custom constructor.
     */
    SensorDataEvent(const float32_t pressure, const float32_t relativeHumidity, const float32_t temperature, const float32_t gasResistance);

    /**
     * Copy constructor.
     * @param other The copied instance.
     */
    SensorDataEvent(const SensorDataEvent& other);

    /**
     * Constructor from deserializer.
     */
    explicit SensorDataEvent(Deserializer& deserializer);

    /**
     * Destructor.
     */
    virtual ~SensorDataEvent(void);

    /**
     * Assignment operator.
     * @param other the assignator instance.
     */
    SensorDataEvent& operator=(const SensorDataEvent& other);

    /**
     * Equality operator.
     */
    bool operator==(const SensorDataEvent& right) const;

    /**
     * Inequality operator.
     */
    bool operator!=(const SensorDataEvent& right) const;

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer);

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const;

    /**
     * Setter for pressure
     */
    void setPressure(const float32_t pressure);

    /**
     * Getter for pressure
     */
    float32_t getPressure(void) const;

    /**
     * Setter for relativeHumidity
     */
    void setRelativeHumidity(const float32_t relativeHumidity);

    /**
     * Getter for relativeHumidity
     */
    float32_t getRelativeHumidity(void) const;

    /**
     * Setter for temperature
     */
    void setTemperature(const float32_t temperature);

    /**
     * Getter for temperature
     */
    float32_t getTemperature(void) const;

    /**
     * Setter for gas resistance
     */
    void setGasResistance(const float32_t gasResistance);

    /**
     * Getter for gas resistance
     */
    float32_t getGasResistance(void) const;

private:

    float32_t m_pressure;
    float32_t m_relativeHumidity;
    float32_t m_temperature;
    float32_t m_gasResistance;
};



#endif // SENSOR_DATA_EVENT_H_
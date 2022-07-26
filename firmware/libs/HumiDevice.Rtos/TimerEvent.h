#ifndef TIMER_EVENT_H_
#define TIMER_EVENT_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventIfc.h>
#include <HumiDevice.Rtos/TimerServiceIfc.h>
#include <HumiDevice.Serialization/Deserializer.h>

/**
 * A RTOS SW timer event which contains the given identifier to compare
 * against epxected TimerId
 */
class TimerEvent : public EventIfc {
public:
    /**
     * Constructor
     */
    explicit TimerEvent(const TimerId id);
    TimerEvent(Deserializer& deserialize);

    /**
     * Destructor
     */
    virtual ~TimerEvent();

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer) override;

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const override;

    // Getter
    float32_t getId() const { return m_id; }

private:
    TimerId m_id; 
};



#endif // TIMER_EVENT_H_
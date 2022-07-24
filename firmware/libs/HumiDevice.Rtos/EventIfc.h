#ifndef EVENTIFC_H_
#define EVENTIFC_H_

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Serialization/SerializableIfc.h>
#include <HumiDevice.Serialization/Deserializer.h>
#include <HumiDevice.Serialization/Serializer.h>

/**
 * A unique event id to deserialize event 
 */
typedef uint8_t EventId;

// no bigger messages are supported
static const size_t MAX_MESSAGE_SIZE = 32 + sizeof(EventId);

/**
 * An event is used to send data or signal to another task 
 */
class EventIfc : public SerializableIfc {
public:

    virtual ~EventIfc() {};

    /**
     * @see SerializableIfc
     */
    virtual void deserialize(Deserializer& deserializer) = 0;

    /**
     * @see SerializableIfc
     */
    virtual void serialize(Serializer& serializer) const = 0;
};


#endif // EVENTIFC_H_
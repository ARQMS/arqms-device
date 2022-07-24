#ifndef EVENTIFC_H_
#define EVENTIFC_H_

// Project includes
#include <HumiPlatform.h>
#include "Rtos/Serialization/SerializableIfc.h"
#include "Rtos/Serialization/Deserializer.h"
#include "Rtos/Serialization/Serializer.h"

/**
 * A unique event id to deserialize event 
 */
typedef uint8_t EventId;

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
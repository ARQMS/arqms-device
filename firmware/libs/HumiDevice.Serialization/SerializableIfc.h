#ifndef SERIALIZABLEIFC_H
#define SERIALIZABLEIFC_H

// Platform
#include <HumiDevice.Platform/Platform.h>

// forward declaration
class Serializer;
class Deserializer;

/**
 * Serialization is the process of translating data structures into a binary representation.
 * This process of serializing an object is also called marshalling an object.
 * The opposite operation, extracting a data structure from a series of bytes, is deserialization.
 */
class SerializableIfc {
public:

    /**
     * Destructor.
     */
    virtual ~SerializableIfc() {}

    /**
     * Deserialize this object from the given deserializer.
     * @param deserializer Deserializer to deserialize this object.
     */
    virtual void deserialize(Deserializer& deserializer) = 0;

    /**
     * Serialize this object into the given serializer.
     * @param serializer Serializer to serialize this object.
     */
    virtual void serialize(Serializer& serializer) const = 0;
};

#endif //#ifndef SERIALIZABLEIFC_H

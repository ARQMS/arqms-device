#ifndef DESERIALIZER_H
#define DESERIALIZER_H

// Project includes
#include <HumiPlatform.h>
#include "Rtos/Serialization/SerializableIfc.h"

/**
 * Deserializes various data types from the given byte buffer.
 * The read position can be set to the beginning by calling reset.
 * Important: The byte buffer passed with the constructor is not copied and only the pointer to the data is stored.
 */
class Deserializer {
public:

    /**
     * Constructor.
     * @param buffer[] Byte buffer array from which data should be read. Must be valid (!= NULL)!
     * @param bufferSize Size of the byte buffer in number of bytes.
     */
    explicit Deserializer(const uint8_t buffer[], const uint32_t bufferSize);

    /**
     * Destructor.
     */
    virtual ~Deserializer();

    /**
     * Returns the read position within the byte buffer.
     * @return Read position within the byte buffer.
     */
    uint32_t getBufferPos() const;

    /**
     * Returns the size of the byte buffer in bytes.
     * @return Size of the byte buffer in bytes.
     */
    uint32_t getBufferSize() const;

    /**
     * Returns a read-only pointer to the byte buffer.
     * @return Read-only pointer to the byte buffer.
     */
    const uint8_t* getBuffer() const;

    /**
     * Returns the remaining data to read from the current position until the buffer is empty.
     * @return The remaining bytes to read.
     */
    uint32_t getUnusedBufferSize() const;

    /**
     * Returns true if the read position is at the beginning (getBufferPos == 0).
     * @return True if the read position is at the beginning.
     */
    bool isReset() const;

    /**
     * Returns true if buffer is completely deserialized.
     * @return True if buffer is completely deserialized.
     */
    bool isFullyDeserialized() const;

    /**
     * Returns the byte of the byte buffer at the given index.
     * @note If the given index is out of bounds, the value at the last position will be returned.
     * @param indexOfByte Byte index.
     * @return Byte at the given index.
     */
    const uint8_t& getBufferByte(const uint32_t indexOfByte) const;

    /**
     * Resets the Deserializer.
     */
    void reset();

    /**
     * Returns a boolean out of the Deserializer.
     * @param data Holds returned boolean.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(bool& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(uint8_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(int8_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(uint16_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(int16_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(uint32_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(int32_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator >> (uint64_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator >> (int64_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(float32_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(float64_t& data);

    /**
     * Returns data out of the Deserializer.
     * @param data Holds returned data.
     * @return Reference to this deserializer.
     */
    Deserializer& operator>>(SerializableIfc& data);

    /**
     * Returns multiple data out of Deserializer.
     * @param pDataDest Holds returned data.
     * @param bytes Number of bytes to return.
     * @return Reference to this deserializer.
     */
    Deserializer& popData(void* pDataDest, uint32_t bytes);

private:

    /**
     * Private default Constructor.
     */
    explicit Deserializer();

    /**
     * Private copy constructor.
     */
    explicit Deserializer(const Deserializer& other);

    /**
     * Private assignment operator.
     */
    Deserializer& operator=(const Deserializer& other);

    //// instance members
    uint32_t m_bufferPos;
    const uint8_t* const m_pBuffer;
    const uint32_t m_bufferSize;
};

#endif //#ifndef DESERIALIZER_H

#ifndef SERIALIZER_H
#define SERIALIZER_H

// Platform
#include <HumiDevice.Platform/Platform.h>

// project includes
#include "SerializableIfc.h"

/**
 * Serializes various data types into the given byte buffer.
 * The read position can be set to the beginning by calling reset.
 * Important: The byte buffer passed with the constructor is storing the actual result.
 */
class Serializer {

public:

    /**
     * Constructor.
     * @param buffer[] Byte buffer array to where the data should be written. Must be valid (!= NULL)!
     * @param bufferSize Size of the byte buffer in number of bytes.
     */
    explicit Serializer(uint8_t buffer[], const uint32_t bufferSize);

    /**
     * Destructor.
     */
    virtual ~Serializer();

    /**
     * Returns the write position within the byte buffer.
     * @return Write position within the byte buffer.
     */
    uint32_t getBufferPos() const;

    /**
     * Sets the write position to the given position.
     * @param newPos New write position.
     */
    void setBufferPos(const uint32_t newPos);

    /**
     * Returns the size of the byte buffer in number of bytes.
     * @return Buffer size in bytes.
     */
    uint32_t getBufferSize() const;

    /**
     * Returns the pointer to the byte buffer.
     * @return Pointer to the write buffer.
     */
    uint8_t* getBuffer();

    /**
     * Returns the remaining data to write from the current position until the buffer is full.
     * @return Remaining buffer size in bytes.
     */
    uint32_t getUnusedBufferSize() const;

    /**
     * States if the serializer is reset (getBufferPos == 0).
     * @return True if the write position is at the beginning.
     */
    bool isReset() const;

    /**
     * States if the buffer is completely serialized.
     * @return True if buffer is completely serialized.
     */
    bool isFullySerialized() const;

    /**
     * Returns the byte of the byte buffer at the given index.
     * @note If the given index is out of bounds, the value at the last position will be returned.
     * @param indexOfByte Index of the byte to return.
     * @return Byte at the given index of the buffer.
     */
    const uint8_t& getBufferByte(const uint32_t indexOfByte) const;

    /**
     * Resets the Serializer.
     */
    void reset();

    /**
     * Puts a bool into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const bool data);

    /**
     * Puts a uint8_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const uint8_t data);

    /**
     * Puts a int8_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const int8_t data);

    /**
     * Puts a uint16_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const uint16_t data);

    /**
     * Puts a int16_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const int16_t data);

    /**
     * Puts a uint32_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const uint32_t data);

    /**
     * Puts a int32_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const int32_t data);

    /**
     * Puts a uint64_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const uint64_t data);

    /**
     * Puts a int64_t into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const int64_t data);

    /**
     * Puts a float into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const float32_t data);

    /**
     * Puts a double into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const float64_t data);

    /**
     * Puts data into the Serializer.
     * @param data Data to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& operator<<(const SerializableIfc& data);
    
    /**
     * Puts data into the Serializer.
     * @param pDataSrc Pointer to the data source.
     * @param bytes Number of bytes to put into the serializer.
     * @return Reference to this serializer.
     */
    Serializer& pushData(const void* pDataSrc, uint32_t bytes);

private:

    /**
     * Default Constructor.
     */
    explicit Serializer();

    /**
     * Private copy constructor.
     */
    explicit Serializer(const Serializer& other);

    /**
     * Private assignment operator.
     */
    Serializer& operator=(const Serializer& other);

    //// instance members
    uint32_t m_bufferPos;
    uint8_t* const m_pBuffer;
    const uint32_t m_bufferSize;
};

#endif //#ifndef SERIALIZER_H

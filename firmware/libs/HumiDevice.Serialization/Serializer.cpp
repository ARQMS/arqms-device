#include "Serializer.h"

Serializer::Serializer(uint8_t buffer[], const uint32_t bufferSize) :
    m_bufferPos(0U),
    m_pBuffer(buffer),
    m_bufferSize(bufferSize)
{
}

Serializer::~Serializer() {
}

uint32_t Serializer::getBufferPos() const {
    return m_bufferPos;
}

uint32_t Serializer::getBufferSize() const {
    return m_bufferSize;
}

uint8_t* Serializer::getBuffer() {
    return m_pBuffer;
}

uint32_t Serializer::getUnusedBufferSize() const {
    return getBufferSize() - getBufferPos();
}

void Serializer::setBufferPos(const uint32_t newPos) {
    if (newPos < m_bufferSize) {
        m_bufferPos = newPos;
    }
}

bool Serializer::isReset() const {
    return getBufferPos() == 0U;
}

bool Serializer::isFullySerialized() const {
    return getUnusedBufferSize() == 0U;
}

const uint8_t& Serializer::getBufferByte(const uint32_t indexOfByte) const {
    if (indexOfByte >= m_bufferSize) {
        return m_pBuffer[m_bufferSize - 1U];
    }
    else {
        return m_pBuffer[indexOfByte];
    }
}

void Serializer::reset() {
    m_bufferPos = 0U;
}

Serializer& Serializer::operator<<(const bool data) {
    const size_t nSizeOfData = 1U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    if (data) {
        m_pBuffer[m_bufferPos] = 1U;
    }
    else {
        m_pBuffer[m_bufferPos] = 0U;
    }

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const uint8_t data) {
    const size_t nSizeOfData = 1U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = data;

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const int8_t data) {
    const size_t nSizeOfData = 1U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(data);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const uint16_t data) {
    const size_t nSizeOfData = 2U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(data >> 8);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(data);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const int16_t data) {
    const size_t nSizeOfData = 2U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(static_cast<uint16_t>(data) >> 8);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(data);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const uint32_t data) {
    const size_t nSizeOfData = 4U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(data >> 24);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(data >> 16);
    m_pBuffer[m_bufferPos + 2U] = static_cast<uint8_t>(data >> 8);
    m_pBuffer[m_bufferPos + 3U] = static_cast<uint8_t>(data);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const int32_t data) {
    const size_t nSizeOfData = 4U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(static_cast<uint32_t>(data) >> 24);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(static_cast<uint32_t>(data) >> 16);
    m_pBuffer[m_bufferPos + 2U] = static_cast<uint8_t>(static_cast<uint32_t>(data) >> 8);
    m_pBuffer[m_bufferPos + 3U] = static_cast<uint8_t>(data);

    m_bufferPos += nSizeOfData; 
    return *this;
}

Serializer& Serializer::operator<<(const uint64_t data) {
    const size_t nSizeOfData = 8U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(data >> 56);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(data >> 48);
    m_pBuffer[m_bufferPos + 2U] = static_cast<uint8_t>(data >> 40);
    m_pBuffer[m_bufferPos + 3U] = static_cast<uint8_t>(data >> 32);
    m_pBuffer[m_bufferPos + 4U] = static_cast<uint8_t>(data >> 24);
    m_pBuffer[m_bufferPos + 5U] = static_cast<uint8_t>(data >> 16);
    m_pBuffer[m_bufferPos + 6U] = static_cast<uint8_t>(data >> 8);
    m_pBuffer[m_bufferPos + 7U] = static_cast<uint8_t>(data);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const int64_t data) {
    const size_t nSizeOfData = 8U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(static_cast<uint64_t>(data) >> 56);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(static_cast<uint64_t>(data) >> 48);
    m_pBuffer[m_bufferPos + 2U] = static_cast<uint8_t>(static_cast<uint64_t>(data) >> 40);
    m_pBuffer[m_bufferPos + 3U] = static_cast<uint8_t>(static_cast<uint64_t>(data) >> 32);
    m_pBuffer[m_bufferPos + 4U] = static_cast<uint8_t>(static_cast<uint64_t>(data) >> 24);
    m_pBuffer[m_bufferPos + 5U] = static_cast<uint8_t>(static_cast<uint64_t>(data) >> 16);
    m_pBuffer[m_bufferPos + 6U] = static_cast<uint8_t>(static_cast<uint64_t>(data) >> 8);
    m_pBuffer[m_bufferPos + 7U] = static_cast<uint8_t>(data);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const float32_t data) {
    const size_t nSizeOfData = 4U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    const uint32_t* const pFloat = reinterpret_cast<const uint32_t*>(&data);
    const uint32_t intCast = *pFloat;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(intCast >> 24);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(intCast >> 16);
    m_pBuffer[m_bufferPos + 2U] = static_cast<uint8_t>(intCast >> 8);
    m_pBuffer[m_bufferPos + 3U] = static_cast<uint8_t>(intCast);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const float64_t data) {
    const size_t nSizeOfData = 8U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    const uint64_t* const pDouble = reinterpret_cast<const uint64_t*>(&data);
    const uint64_t intCast = *pDouble;

    m_pBuffer[m_bufferPos] = static_cast<uint8_t>(intCast >> 56);
    m_pBuffer[m_bufferPos + 1U] = static_cast<uint8_t>(intCast >> 48);
    m_pBuffer[m_bufferPos + 2U] = static_cast<uint8_t>(intCast >> 40);
    m_pBuffer[m_bufferPos + 3U] = static_cast<uint8_t>(intCast >> 32);
    m_pBuffer[m_bufferPos + 4U] = static_cast<uint8_t>(intCast >> 24);
    m_pBuffer[m_bufferPos + 5U] = static_cast<uint8_t>(intCast >> 16);
    m_pBuffer[m_bufferPos + 6U] = static_cast<uint8_t>(intCast >> 8);
    m_pBuffer[m_bufferPos + 7U] = static_cast<uint8_t>(intCast);

    m_bufferPos += nSizeOfData;
    return *this;
}

Serializer& Serializer::operator<<(const SerializableIfc& data) {
    data.serialize(*this);
    return *this;
}

Serializer& Serializer::pushData(const void* const pDataSrc, const uint32_t bytes) {
    if ((pDataSrc == NULL) || ((m_bufferPos + bytes) > m_bufferSize)) return *this;

    memcpy(&m_pBuffer[m_bufferPos], pDataSrc, bytes); //lint !e534 //ignoring return value of function 'memcpy'

    m_bufferPos += bytes;
    return *this;
}

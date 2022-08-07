#include "Deserializer.h"

Deserializer::Deserializer(const uint8_t buffer[], const uint32_t bufferSize) :
    m_bufferPos(0U),
    m_pBuffer(buffer),
    m_bufferSize(bufferSize) {
}

Deserializer::~Deserializer() {
}

uint32_t Deserializer::getBufferPos() const {
    return m_bufferPos;
}

uint32_t Deserializer::getBufferSize() const {
    return m_bufferSize;
}

const uint8_t* Deserializer::getBuffer() const {
    return m_pBuffer;
}

uint32_t Deserializer::getUnusedBufferSize() const {
    return getBufferSize() - getBufferPos();
}

bool Deserializer::isReset() const {
    return getBufferPos() == 0U;
}

bool Deserializer::isFullyDeserialized() const {
    return getUnusedBufferSize() == 0U;
}

const uint8_t& Deserializer::getBufferByte(const uint32_t indexOfByte) const {
    if (indexOfByte >= m_bufferSize) {
        return m_pBuffer[(m_bufferSize - 1U)];
    }

    return m_pBuffer[indexOfByte];
}

void Deserializer::reset() {
    m_bufferPos = 0U;
}

Deserializer& Deserializer::operator>>(bool& data) {
    // booleans are represented as bytes. A value of 0 = false; everything else = true
    const size_t nSizeOfData = 1U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    data = m_pBuffer[m_bufferPos] != 0U;

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(char8_t& data) {
    const size_t nSizeOfData = 1U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;
        
    data = m_pBuffer[m_bufferPos];
    
    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(uint8_t& data) {
    const size_t nSizeOfData = 1U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;
        
    data = m_pBuffer[m_bufferPos];
    
    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(int8_t& data) {
    const size_t nSizeOfData = 1U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    data = static_cast<int8_t>(m_pBuffer[m_bufferPos]);

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(uint16_t& data) {
    const size_t nSizeOfData = 2U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    data = 0U;
    data |= m_pBuffer[m_bufferPos];
    data <<= 8;
    data |= m_pBuffer[(m_bufferPos+1U)];

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(int16_t& data) {
    const size_t nSizeOfData = 2U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;
    data = 0;
    data |= static_cast<int16_t>(m_pBuffer[m_bufferPos]);
    data <<= 8U;
    data |= static_cast<int16_t>(m_pBuffer[(m_bufferPos+1U)]);

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(uint32_t& data) {
    const size_t nSizeOfData = 4U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;
    data = 0U;
    data |= static_cast<uint32_t>(m_pBuffer[m_bufferPos]);
    data <<= 8U;
    data |= static_cast<uint32_t>(m_pBuffer[m_bufferPos+1U]);
    data <<= 8U;
    data |= static_cast<uint32_t>(m_pBuffer[m_bufferPos+2U]);
    data <<= 8U;
    data |= static_cast<uint32_t>(m_pBuffer[m_bufferPos+3U]);

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(int32_t& data) {
    const size_t nSizeOfData = 4U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    data = 0;
    data |= static_cast<int32_t>(m_pBuffer[m_bufferPos]);
    data <<= 8U;
    data |= static_cast<int32_t>(m_pBuffer[m_bufferPos+1U]);
    data <<= 8U;
    data |= static_cast<int32_t>(m_pBuffer[m_bufferPos+2U]);
    data <<= 8U;
    data |= static_cast<int32_t>(m_pBuffer[m_bufferPos+3U]);

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(uint64_t& data) {
    const size_t nSizeOfData = 8U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    data = 0U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos]);
    data <<= 8U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos + 1U]);
    data <<= 8U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos + 2U]);
    data <<= 8U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos + 3U]);
    data <<= 8U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos + 4U]);
    data <<= 8U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos + 5U]);
    data <<= 8U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos + 6U]);
    data <<= 8U;
    data |= static_cast<uint64_t>(m_pBuffer[m_bufferPos + 7U]);

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator >> (int64_t& data) {
    const size_t nSizeOfData = 8U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;

    data = 0;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos]);
    data <<= 8U;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos + 1U]);
    data <<= 8U;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos + 2U]);
    data <<= 8U;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos + 3U]);
    data <<= 8U;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos + 4U]);
    data <<= 8U;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos + 5U]);
    data <<= 8U;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos + 6U]);
    data <<= 8U;
    data |= static_cast<int64_t>(m_pBuffer[m_bufferPos + 7U]);

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(float32_t& data) {
    const size_t nSizeOfData = 4U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;
    uint32_t value = 0U;
    value |= m_pBuffer[m_bufferPos];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+1U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+2U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+3U];
    const float32_t* const pFloat = reinterpret_cast<float32_t*>(&value);
    data = *pFloat;

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(float64_t& data) {
    const size_t nSizeOfData = 8U;
    if ((m_bufferPos + nSizeOfData) > m_bufferSize) return *this;
    uint64_t value = 0U;
    value |= m_pBuffer[m_bufferPos];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+1U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+2U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+3U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+4U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+5U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+6U];
    value <<= 8;
    value |= m_pBuffer[m_bufferPos+7U];

    const float64_t* const pDouble = reinterpret_cast<float64_t*>(&value);
    data = *pDouble;

    m_bufferPos += nSizeOfData;
    return *this;
}

Deserializer& Deserializer::operator>>(SerializableIfc& data) {
    data.deserialize(*this);
    return *this;
}

Deserializer& Deserializer::popData(void* const pDataDest, const uint32_t bytes) {
    if ((pDataDest == NULL) || ((m_bufferPos + bytes) > m_bufferSize)) return *this;

    memcpy(pDataDest, &getBufferByte(m_bufferPos), bytes); //lint !e534 //ignoring return value of function 'memcpy'
    m_bufferPos += bytes;
    
    return *this;
}

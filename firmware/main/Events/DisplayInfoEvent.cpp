#include "DisplayInfoEvent.h"

DisplayInfoEvent::DisplayInfoEvent(void) :
    m_status(false) {
    // Nothing to do.
}

DisplayInfoEvent::DisplayInfoEvent( const bool status) :
    m_status(status) {
    // Nothing to do.
}

DisplayInfoEvent::DisplayInfoEvent(const DisplayInfoEvent& other) :
    m_status(other.m_status) {
    // Nothing to do.
}

DisplayInfoEvent::DisplayInfoEvent(Deserializer& deserializer) :
    m_status(false) {
    DisplayInfoEvent::deserialize(deserializer);
}

DisplayInfoEvent::~DisplayInfoEvent(void) {
    // Nothing to do.
}

DisplayInfoEvent& DisplayInfoEvent::operator=(const DisplayInfoEvent& other) {
    if (&other == this) {
        return *this;
    }

    m_status = other.m_status;
    return *this;
}

bool DisplayInfoEvent::operator==(const DisplayInfoEvent& right) const {
    return (this->m_status == right.m_status);
}

bool DisplayInfoEvent::operator!=(const DisplayInfoEvent& right) const {
    return !(*this == right);
}

void DisplayInfoEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_status;
}

void DisplayInfoEvent::serialize(Serializer& serializer) const {
    serializer << m_status;
}

void DisplayInfoEvent::setStatus(const bool status) {
    m_status = status;
}

bool DisplayInfoEvent::getStatus(void) const {
    return m_status;
}

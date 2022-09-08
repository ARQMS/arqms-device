#include "ButtonEvent.h"

ButtonEvent::ButtonEvent(void) :
    m_id(ButtonId::USER),
    m_status(ButtonStatus::SHORT_PRESS) {
    // Nothing to do.
}

ButtonEvent::ButtonEvent(const ButtonId id, const ButtonStatus status) :
    m_id(id),
    m_status(status) {
    // Nothing to do.
}

ButtonEvent::ButtonEvent(const ButtonEvent& other) :
    m_id(other.m_id),
    m_status(other.m_status) {
    // Nothing to do.
}

ButtonEvent::ButtonEvent(Deserializer& deserializer) :
    m_id(ButtonId::USER),
    m_status(ButtonStatus::SHORT_PRESS) {
    ButtonEvent::deserialize(deserializer);
}

ButtonEvent::~ButtonEvent(void) {
    // Nothing to do.
}

ButtonEvent& ButtonEvent::operator=(const ButtonEvent& other) {
    if (&other == this) {
        return *this;
    }
    m_id = other.m_id;
    m_status = other.m_status;
    return *this;
}

bool ButtonEvent::operator==(const ButtonEvent& right) const {
    return (this->m_id == right.m_id) &&
        (this->m_status == right.m_status);
}

bool ButtonEvent::operator!=(const ButtonEvent& right) const {
    return !(*this == right);
}

void ButtonEvent::deserialize(Deserializer& deserializer) {
    uint8_t id;
    deserializer >> id;
    m_id = static_cast<ButtonId>(id);
    uint8_t status;
    deserializer >> status;
    m_status = static_cast<ButtonStatus>(status);
}

void ButtonEvent::serialize(Serializer& serializer) const {
    serializer << static_cast<uint8_t>(m_id);
    serializer << static_cast<uint8_t>(m_status);
}

void ButtonEvent::setButtonId(const ButtonId id) {
    m_id = id;
}

ButtonId ButtonEvent::getButtonId(void) const {
    return m_id;
}

void ButtonEvent::setStatus(const ButtonStatus status) {
    m_status = status;
}

ButtonStatus ButtonEvent::getStatus(void) const {
    return m_status;
}

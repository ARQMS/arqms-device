#include "TimerEvent.h"

TimerEvent::TimerEvent(const TimerId id) :
    m_id(id) {
}

TimerEvent::TimerEvent(Deserializer& deserializer) {
    deserialize(deserializer);
}

TimerEvent::~TimerEvent() {
}

void TimerEvent::deserialize(Deserializer& deserializer) {
    deserializer >> m_id;
}

void TimerEvent::serialize(Serializer& serializer) const {
    serializer << m_id;
}
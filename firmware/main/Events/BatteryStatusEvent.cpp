#include "BatteryStatusEvent.h"

BatteryStatusEvent::BatteryStatusEvent(void):
    m_level(0.0){

}

BatteryStatusEvent::BatteryStatusEvent(uint8_t level):
    m_level(level){

}

BatteryStatusEvent::BatteryStatusEvent(const BatteryStatusEvent& other):
    m_level(other.m_level){
    
}

BatteryStatusEvent::BatteryStatusEvent(Deserializer& deserializer){
    BatteryStatusEvent::deserialize(deserializer);
}

BatteryStatusEvent::~BatteryStatusEvent(void){
    // Nothing to do
}

BatteryStatusEvent& BatteryStatusEvent::operator=(const BatteryStatusEvent& other){
    if(&other == this){
        return *this;
    }
    m_level = other.m_level;
    return *this;
}

bool BatteryStatusEvent::operator==(const BatteryStatusEvent& right) const{
    return this->m_level == right.m_level;
}

bool BatteryStatusEvent::operator!=(const BatteryStatusEvent& right) const{
    return !(*this == right);
}

void BatteryStatusEvent::deserialize(Deserializer& deserializer){
    deserializer >> m_level;
}

void BatteryStatusEvent::serialize(Serializer& serializer) const{
    serializer << m_level;
}


void BatteryStatusEvent::setLevel(uint8_t level){
    m_level = level;
}

uint8_t BatteryStatusEvent::getLevel(){
    return m_level;
}

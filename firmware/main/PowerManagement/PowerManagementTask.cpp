#include "PowerManagementTask.h"

#include "Drivers/ApplicationHardwareConfig.h"
#include "Events/EventIdentifiers.h"

PowerManagementTask::PowerManagementTask():
m_pRefreshTimer(NULL){
}

PowerManagementTask::~PowerManagementTask(){
}

void PowerManagementTask::onInitialize(){
    m_pRefreshTimer = createPeriodicTimer(REFRESH_RATE);
    m_adc.initialize();
}

void PowerManagementTask::onStart(){
    m_pRefreshTimer->start();
}

void PowerManagementTask::onHandleEvent(EventId eventId, Deserializer* pEvent){
    // Nothing to do
}

void PowerManagementTask::onHandleTimer(const TimerId timerId){
    int value = m_adc.readADC();
    BatteryStatusEvent status(value);
    Control.send(EventIdentifiers::BATTERY_LEVEL_EVENT, &status);
}

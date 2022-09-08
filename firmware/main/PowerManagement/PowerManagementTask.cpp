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
}

void PowerManagementTask::onStart(){
    m_pRefreshTimer->start();
}

void PowerManagementTask::onHandleEvent(EventId eventId, Deserializer* pEvent){
    // Nothing to do
}

void PowerManagementTask::onHandleTimer(const TimerId timerId){
    BatteryStatusEvent status(100);
    Control.send(EventIdentifiers::BATTERY_LEVEL_EVENT, &status);
}

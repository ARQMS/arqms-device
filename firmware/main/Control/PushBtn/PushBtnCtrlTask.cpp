#include "PushBtnCtrlTask.h"

// IDF include
#include <driver/gpio.h>

// Application
#include <Drivers/ApplicationHardwareConfig.h>
#include <Events/EventIdentifiers.h>


PushBtnCtrlTask::PushBtnCtrlTask() :
    Control() {
}

PushBtnCtrlTask::~PushBtnCtrlTask() {
}

void PushBtnCtrlTask::onInitialize() {
    // nothing to do
}

void PushBtnCtrlTask::onStart() {
    // nothing to do
}

void PushBtnCtrlTask::onHandleEvent(EventId eventId, Deserializer* pEvent) {
    ESP_LOGI("PushBtn", "receive msg %i", eventId);

    switch (eventId) {
    case EventIdentifiers::BTN_CTRL_EVENT:
        onHandleButtonEvent(ButtonEvent(*pEvent));
        break;
    
    default:
        break;
    }
}

void PushBtnCtrlTask::onHandleTimer(TimerId timerId) {
    // TODO
}

void PushBtnCtrlTask::onHandleButtonEvent(const ButtonEvent& event) {
    ESP_LOGI("PushBtn", "Pressed BTN %i: %i", event.getButtonId(), event.getStatus());
}

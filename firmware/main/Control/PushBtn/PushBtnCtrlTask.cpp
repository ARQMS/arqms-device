#include "PushBtnCtrlTask.h"

// IDF include
#include <driver/gpio.h>

// Application
#include <Drivers/ApplicationHardwareConfig.h>
#include <Events/EventIdentifiers.h>

PushBtnCtrlTask::PushBtnCtrlTask() :
    Control(),
    m_swRstBtn(ButtonId::SW_RESET, *this),
    m_usrBtn(ButtonId::USER, *this) {
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
    switch (eventId) {
    case EventIdentifiers::DEVICE_BTN_EVENT:
        onHandleButtonEvent(ButtonEvent(*pEvent));
        break;
    }
}

void PushBtnCtrlTask::onHandleTimer(TimerId timerId) {
    // nothing to do
}

void PushBtnCtrlTask::onHandleButtonEvent(const ButtonEvent& event) {
    switch (event.getButtonId()) {
        case ButtonId::USER: 
            m_usrBtn.onStateChanged();
            break;

        case ButtonId::SW_RESET: 
            m_swRstBtn.onStateChanged();
            break;

        default:
            ESP_LOGW("BtnCtrl", "PushBtnCtrlTask can not handle ButtonId %i", event.getButtonId());
            break;
    }
}

void PushBtnCtrlTask::onButtonPressed(const ButtonId id, const ButtonStatus status) {
    ButtonEvent event(id, status);
    Control.send(EventIdentifiers::DEVICE_BTN_EVENT, &event);
}

#include "GpioIrqHandler.h"

// IDF include
#include <driver/gpio.h>

// Application
#include <Drivers/ApplicationHardwareConfig.h>
#include <Events/EventIdentifiers.h>
#include <Events/ButtonEvent.h>

GpioIrqHandler GpioIrqHandler::s_pInstance;

GpioIrqHandler& GpioIrqHandler::getInstance() {
    return s_pInstance;
}

GpioIrqHandler::GpioIrqHandler() :
    PushBtn() {
}

GpioIrqHandler::~GpioIrqHandler() {
}

void GpioIrqHandler::onGpioIsr(void* arg) {
    const uint32_t num = (uint32_t)arg;
    EventPublisherSingle& sender = GpioIrqHandler::getInstance().PushBtn;

    ButtonEvent event(static_cast<ButtonId>(num), ButtonStatus::SHORT_PRESS);
    sender.send(EventIdentifiers::BTN_CTRL_EVENT, &event);
}

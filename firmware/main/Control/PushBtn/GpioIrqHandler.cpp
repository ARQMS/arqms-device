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
    const gpio_num_t gpioNum = static_cast<gpio_num_t>((uint32_t)arg);
    
    // gpio_get_level(gpioNum) does not return always correct gpio level
    // se we send always a SHORT_PRESS state and handle it by software
    ButtonEvent event(static_cast<ButtonId>(gpioNum), ButtonStatus::SHORT_PRESS);

    EventPublisherSingle& sender = GpioIrqHandler::getInstance().PushBtn;
    sender.send(EventIdentifiers::BTN_CTRL_EVENT, &event);
}

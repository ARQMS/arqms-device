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

    // Strange behavior, some times interrupt is fired twice, even osczilloscope can not detect any
    // spikes. When a log is written, this issue does not occure. So it seems it in't a hardware issue.
    // Furthermore, 'Queue full. No eventid %i to 0x00000040 sent' is written as error to output without
    // the following log which indicates interrupt is called many times...
    //
    // For the moment, log any interrupt and avoid strange behavior
    ESP_DRAM_LOGI("Btn", "Gpio ISR Edge detected");
}

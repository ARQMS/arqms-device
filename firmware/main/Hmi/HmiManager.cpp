#include "HmiManager.h"

#include "Events/Events.h"
#include "driver/gpio.h"

#define GPIO_RST_BTN        GPIO_NUM_2
#define GPIO_USR_BTN        GPIO_NUM_3
#define GPIO_MOD_LED        GPIO_NUM_10
#define GPIO_GOOD_LED       GPIO_NUM_20
#define GPIO_POOR_LED       GPIO_NUM_21

// ISR Handler for GPIO3
extern "C" void UsrPressedIsr(void* arg) {
    EventLoopIfc* publisher = static_cast<EventLoopIfc*>(arg);
    publisher->sendEventIsr(HMI_EVENTS, USR_BTN_PRESSED);
}

// ISR Handler for GPIO2
extern "C" void RstPressedIsr(void* arg) {
    EventLoopIfc* publisher = static_cast<EventLoopIfc*>(arg);
    publisher->sendEventIsr(HMI_EVENTS, RST_BTN_PRESSED);
}

HmiManager::HmiManager(EventLoopIfc& eventLoop) : 
    eventLoop(eventLoop) {
}

void HmiManager::initialize() {
    // GPIO inputs configuration
    gpio_config_t gpioCfg = {};
    gpioCfg.intr_type = GPIO_INTR_NEGEDGE;
    gpioCfg.mode = GPIO_MODE_INPUT;
    gpioCfg.pull_up_en = GPIO_PULLUP_ENABLE;
    gpioCfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioCfg.pin_bit_mask = GPIO_RST_BTN | GPIO_USR_BTN;
    gpio_config(&gpioCfg);

    // GPIO Interrupt configuration
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(GPIO_RST_BTN, RstPressedIsr, &eventLoop);
    gpio_isr_handler_add(GPIO_USR_BTN, UsrPressedIsr, &eventLoop);

    // GPIO outputs configuration
    gpioCfg.intr_type = GPIO_INTR_DISABLE;
    gpioCfg.mode = GPIO_MODE_OUTPUT_OD;
    gpioCfg.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioCfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioCfg.pin_bit_mask = GPIO_MOD_LED | GPIO_GOOD_LED | GPIO_POOR_LED;
    gpio_config(&gpioCfg);
}

void HmiManager::onEvent(esp_event_base_t base, int32_t id, void* data) {
    if (base == HMI_EVENTS && id == USR_BTN_PRESSED) {
        onUsrPressed();
    }
    else if (base == TIMER_EVENTS && id == TIMER_ELAPSED) {
        TimerId* pId = static_cast<TimerId*>(data);
        if (*pId == resetTimerId) {
            onResetTimer();
        }
    }
}

void HmiManager::onUsrPressed() {
    gpio_set_level(GPIO_GOOD_LED, true);
    gpio_set_level(GPIO_MOD_LED, true);
    gpio_set_level(GPIO_POOR_LED, true);

    // start reset timer
    resetTimerId = eventLoop.startOneShotTimer(10 * 1000);
}

void HmiManager::onResetTimer() {
    gpio_set_level(GPIO_GOOD_LED, false);
    gpio_set_level(GPIO_MOD_LED, false);
    gpio_set_level(GPIO_POOR_LED, false);
}
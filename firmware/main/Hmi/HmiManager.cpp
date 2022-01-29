#include "HmiManager.h"

#include "driver/gpio.h"

#define GPIO_RST_BTN        GPIO_NUM_2
#define GPIO_USR_BTN        GPIO_NUM_3
#define GPIO_MOD_LED        GPIO_NUM_10
#define GPIO_GOOD_LED       GPIO_NUM_20
#define GPIO_POOR_LED       GPIO_NUM_21

volatile static bool swRstClicked = false;
volatile static bool usrClicked = false;
volatile static bool timerCalled = false;

// ISR Handler for GPIO2
extern "C" void gpio_isr(void* arg) {
    uint32_t gpio_num = (uint32_t)arg;
    switch (gpio_num) {
        case GPIO_RST_BTN:         swRstClicked = true; break;
        case GPIO_USR_BTN:         usrClicked   = true; break;
    }
}

HmiManager::HmiManager() {
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
    gpio_isr_handler_add(GPIO_RST_BTN, gpio_isr, (void*)GPIO_RST_BTN);
    gpio_isr_handler_add(GPIO_USR_BTN, gpio_isr, (void*)GPIO_USR_BTN);

    // GPIO outputs configuration
    gpioCfg.intr_type = GPIO_INTR_DISABLE;
    gpioCfg.mode = GPIO_MODE_OUTPUT_OD;
    gpioCfg.pull_up_en = GPIO_PULLUP_DISABLE;
    gpioCfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioCfg.pin_bit_mask = GPIO_MOD_LED | GPIO_GOOD_LED | GPIO_POOR_LED;
    gpio_config(&gpioCfg);
}


void HmiManager::onEvent(esp_event_base_t base, int32_t id, void* data) {

}

void HmiManager::update() {
    if (usrClicked) {
        gpio_set_level(GPIO_GOOD_LED, true);
        gpio_set_level(GPIO_MOD_LED, true);
        gpio_set_level(GPIO_POOR_LED, true);

        // TODO 
        //  Var1: start timer and set timerCalled on callback
        //  Var2: only call update on any change by eventLoop
    } 
    else if (usrClicked && timerCalled) {
        gpio_set_level(GPIO_GOOD_LED, false);
        gpio_set_level(GPIO_MOD_LED, false);
        gpio_set_level(GPIO_POOR_LED, false);

        usrClicked = false;
    }

    if (swRstClicked) {
        // TODO

        swRstClicked = false;
    }
}

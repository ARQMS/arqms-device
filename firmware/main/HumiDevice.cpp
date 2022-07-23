// Idf includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Project includes
#include <HumiPlatform.h>
#include "Drivers/ApplicationHardwareConfig.h"
#include "Drivers/StartupController.h"

/**
 * The entry point for humi device. This is called after second bootloader has 
 * initialized all peripherals pins and c++/c domain.
 * 
 * It is configured as a FreeRTOS task.
 */
extern "C" void app_main(void) {
    // initialize hardware
    StartupController::initializeCPU();
    StartupController::initializeGpio();
    StartupController::initializeSpi1();
    StartupController::initializeAdc1();

    uint8_t ledState = 0;
    while(true){
        gpio_set_level(LED_AIR_POOR, ledState);
        ledState = !ledState;
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}


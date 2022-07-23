// Idf includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Project includes
#include <HumiPlatform.h>
#include "Drivers/ApplicationHardwareConfig.h"
#include "Drivers/AirIndicatorDriver.h"
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

    // TODO remove test code
    AirIndicatorDriver airIndicator(LED_AIR_GOOD, LED_AIR_MOD, LED_AIR_POOR);
    AirIndicatorDriver::Quality quality = AirIndicatorDriver::Quality::UNKNOWN;
    while(true) {
        airIndicator.setQuality(quality);
        quality = static_cast<AirIndicatorDriver::Quality>((quality + 1) % 4);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


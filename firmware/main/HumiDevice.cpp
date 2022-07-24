// FreeRTOS includes
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Project includes
#include <HumiPlatform.h>
#include "Rtos/EventRuntime.h"
#include "Drivers/ApplicationHardwareConfig.h"

#include "Drivers/StartupController.h"
#include "Control/ControlTask.h"
#include "GuiUpdater/GuiUpdaterTask.h"

// Create task definitions
CREATE_TASK_DEF(Control, 0x800, 10) // 2kB Stack
CREATE_TASK_DEF(GuiUpdater, 0x800, 10) // 2kB Stack

/**
 * The entry point for humi device. This is called after second bootloader has 
 * initialized all peripherals pins and c++/c domain.
 * 
 * It is configured as a FreeRTOS task.
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/freertos.html#freertos-applications
 */
extern "C" void app_main(void) {
    ESP_LOGI("HumiDevice", "Application initialization...");

    // initialize hardware
    StartupController::initializeCPU();
    StartupController::initializeGpio();
    StartupController::initializeSpi1();
    StartupController::initializeAdc1();

    // create tasks
    ControlTask& control = *createControlTask();
    GuiUpdaterTask& guiUpdater = *createGuiUpdaterTask();

    // connect tasks
    control.GuiUpdater.connect(guiUpdater);
    control.Same.connect(control);

    // start tasks
    startGuiUpdaterTask();
    startControlTask();
}


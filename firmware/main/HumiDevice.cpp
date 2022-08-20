// FreeRTOS includes
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Platform
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventRuntime.h>

// Project includes
#include "Drivers/ApplicationHardwareConfig.h"
#include "Drivers/StartupController.h"
#include "Control/ControlTask.h"
#include "GuiUpdater/GuiUpdaterTask.h"
#include "CloudLink/CloudLinkTask.h"
#include "PowerManagement/PowerManagementTask.h"

// Create task definitions
CREATE_TASK_DEF(Control, 0x8000, 10) // 4kB Stack
CREATE_TASK_DEF(GuiUpdater, 0x800, 10) // 2kB Stack
CREATE_TASK_DEF(CloudLink, 0x8000, 11) // 4kB Stack
CREATE_TASK_DEF(PowerManagement, 0x800, 11) // 

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
    StartupController::initialize();
    StartupController::initializeGpio();
    StartupController::initializeSpi1();
    StartupController::initializeAdc1();

    // create tasks
    ControlTask& control = *createControlTask();
    GuiUpdaterTask& guiUpdater = *createGuiUpdaterTask();
    CloudLinkTask& cloudLink = *createCloudLinkTask();
    PowerManagementTask& powerManagement = *createPowerManagementTask();

    // connect tasks
    control.GuiSettings.connect(guiUpdater);
    control.WifiSettings.connect(cloudLink);
    cloudLink.StatusEvent.connect(control);
    cloudLink.StatusEvent.connect(guiUpdater);
    powerManagement.Control.connect(control);

    // start tasks
    startGuiUpdaterTask();
    startControlTask();
    startCloudLinkTask();
    startPowerManagementTask();
}


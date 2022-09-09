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
#include "Control/PushBtn/PushBtnCtrlTask.h"
#include "Control/PushBtn/GpioIrqHandler.h"
#include "GuiUpdater/GuiUpdaterTask.h"
#include "CloudLink/CloudLinkTask.h"
#include "MeasSensor/MeasSensorTask.h"
#include "MeasFilter/MeasFilterTask.h"
#include "PowerManagement/PowerManagementTask.h"

// Create task definitions
CREATE_TASK_DEF(Control, 0xC00, 10) // 3kB Stack
CREATE_TASK_DEF(GuiUpdater, 0xC00, 10) // 3kB Stack
CREATE_TASK_DEF(MeasSensor, 0xC00, 10) // 3kB Stack
CREATE_TASK_DEF(MeasFilter, 0xC00, 10) // 3kB Stack
CREATE_TASK_DEF(CloudLink, 0x8000, 11) // 4kB Stack
CREATE_TASK_DEF(PushBtnCtrl, 0xC00, 9) // 3kB Stack
CREATE_TASK_DEF(PowerManagement, 0x800, 11) // 2kB Stack

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
    StartupController::initializeIrq();

    // create tasks
    ControlTask& control = *createControlTask();
    GuiUpdaterTask& guiUpdater = *createGuiUpdaterTask();
    CloudLinkTask& cloudLink = *createCloudLinkTask();
    MeasSensorTask& measSensor = *createMeasSensorTask();
    MeasFilterTask& measFilter = *createMeasFilterTask();
    PowerManagementTask& powerManagement = *createPowerManagementTask();
    PushBtnCtrlTask& btnCtrl = *createPushBtnCtrlTask();
    GpioIrqHandler& gpioHandler = GpioIrqHandler::getInstance();

    // connect tasks
    control.Gui.connect(guiUpdater);
    control.CloudLink.connect(cloudLink);
    control.MeasSensor.connect(measSensor);
    control.MeasFilter.connect(measFilter);
    guiUpdater.Control.connect(control);
    cloudLink.Status.connect(control);
    cloudLink.Status.connect(guiUpdater);
    cloudLink.Control.connect(control);
    measSensor.Measurement.connect(measFilter);
    measSensor.Status.connect(guiUpdater);
    measFilter.Measurement.connect(cloudLink);
    measFilter.Measurement.connect(guiUpdater);
    measFilter.Measurement.connect(control);
    gpioHandler.PushBtn.connect(btnCtrl);
    btnCtrl.Control.connect(control);
    btnCtrl.Control.connect(guiUpdater);
    powerManagement.Control.connect(control);

    // start tasks
    startGuiUpdaterTask();
    startControlTask();
    startCloudLinkTask();
    startMeasSensorTask();
    startMeasFilterTask();
    startPushBtnCtrlTask();
    startPowerManagementTask();
}


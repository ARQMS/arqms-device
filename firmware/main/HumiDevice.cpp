// Project includes
#include <HumiPlatform.h>
#include "Rtos/TaskFactory.h"
#include "Drivers/ApplicationHardwareConfig.h"
#include "Drivers/AirIndicatorDriver.h"
#include "Drivers/StartupController.h"
#include "Control/ControlTask.h"


CREATE_TASK_DEF(Control, 200)

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

    // create tasks
    // MeasurementFilterTask measFilter();
    // GuiUpdaterTask guiUpdater();
    // PowerMgmtTask powerMgmt();
    // CloudLinkTask cloudLink();
    ControlTask& control = *createControlTask();

    // MeasurementSensorTask measSensor();

    // control.MeasSensor.connect(measSensor);

    /**
     * in MeasSensorTask:
     * execute(EventId eventId, byte* data)
     *      switch (eventId) {
     *          case HumiDeviceEvents::Snapshot: Snapshot s(data); handleSnapshot(s); break; // used google protobuf
     *          ...
     *      }
     * 
     * handleSnapshot(Snapshot s)
     *      MeasFilter.publish(data);
     */

    // TODO remove test code
    AirIndicatorDriver airIndicator(LED_AIR_GOOD, LED_AIR_MOD, LED_AIR_POOR);
    AirIndicatorDriver::Quality quality = AirIndicatorDriver::Quality::UNKNOWN;
    while(true) {
        airIndicator.setQuality(quality);
        quality = static_cast<AirIndicatorDriver::Quality>((quality + 1) % 4);


        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


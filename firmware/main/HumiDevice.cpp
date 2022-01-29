#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "DeviceState/DeviceStateMachine.h"
#include "DeviceStorage/DeviceStorage.h"
#include "WiFi/WiFiManager.h"

/**
 * The entry point for humi device. This is called after second bootloader has 
 * initialized all peripherals pins and c++/c domain.
 * 
 * It is configured as a FreeRTOS task.
 *  
 */
extern "C" void app_main(void) {
    static DeviceStorage storage;
    static WiFiManager wifiManager(storage);

    static DeviceHandler stateMachineHandler(wifiManager, storage);
    static DeviceStateMachine stateMachine(stateMachineHandler);

    // initial Wake up source
    // TODO

    // initial Hardware
    stateMachine.initialize();
    storage.initialize();
    wifiManager.initialize();

    // initial IRQ
    // TODO

    // start Task:
    // 	    HMITask
    // 	    MQTTTask

    while (true) {
        stateMachine.process();

        // delay for 1 ms. Ensure idle hook is called
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

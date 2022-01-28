#include "DeviceState/DeviceStateMachine.h"
#include "DeviceStorage/DeviceStorage.h"
#include "WiFi/WiFiManager.h"

static DeviceStorage storage;
static DeviceStateMachine stateMachine;
static WiFiManager wifiManager;

/**
 * @brief The entry point for humi device. This is called after second bootloader has 
 * initialized all peripherals pins and c++/c domain.
 * 
 * It is configured as a FreeRTOS task.
 *  
 */
extern "C" void app_main(void) {
    // initial Wake up source

    // initial Hardware (NVS)
    storage.initialize();
    wifiManager.initialize();

    // initial IRQ

    // start Task:
    // 	    HMITask
    // 	    MQTTTask

    stateMachine.reset();

    while (true) {
        stateMachine.process();
    }
}

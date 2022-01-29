// ESP-IDF
#include "esp_event.h"

// Application
#include "DeviceStorage/DeviceStorage.h"
#include "WiFi/WiFiManager.h"
#include "Sensor/SensorManager.h"
#include "Hmi/HmiManager.h"
#include "DeviceState/DeviceStateMachine.h"
#include "Events/EventDispatcher.h"
#include "Events/Events.h"

#define SM_STACK_SIZE configMINIMAL_STACK_SIZE
#define HMI_STACK_SIZE configMINIMAL_STACK_SIZE

static DeviceStorage storage;
static WiFiManager wifi(storage);
static EventDispatcher dispatcher;
static HmiManager hmi(dispatcher);
static SensorManager sensor(dispatcher);

static DeviceHandler stateMachineHandler(wifi, sensor, storage);
static DeviceStateMachine stateMachine(stateMachineHandler);

// Event loop
ESP_EVENT_DEFINE_BASE(HMI_EVENTS);
ESP_EVENT_DEFINE_BASE(SENSOR_EVENTS);
ESP_EVENT_DEFINE_BASE(TIMER_EVENTS);

extern "C" void statemachine_main(void* args) {
    DeviceStateMachine* pSm = static_cast<DeviceStateMachine*>(args);

    while (true) {
        pSm->process();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

/**
 * The entry point for humi device. This is called after second bootloader has 
 * initialized all peripherals pins and c++/c domain.
 * 
 * It is configured as a FreeRTOS task.
 */
extern "C" void app_main(void) {
    // initial Hardware
    stateMachine.initialize();
    storage.initialize();
    wifi.initialize();
    hmi.initialize();
    dispatcher.initialize();

    // register events
    dispatcher.registerEventHandler(TIMER_EVENTS, TIMER_ELAPSED, hmi);
    dispatcher.registerEventHandler(HMI_EVENTS, ESP_EVENT_ANY_ID, hmi);
    dispatcher.registerEventHandler(SENSOR_EVENTS, DATA_ACQUIRED, hmi);

    // application tasks
    xTaskCreate(statemachine_main, "StateMachineTask", SM_STACK_SIZE, &stateMachine, uxTaskPriorityGet(NULL), NULL);
}


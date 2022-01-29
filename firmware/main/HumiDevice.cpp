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
static SensorManager sensor;
static HmiManager hmi;
static EventDispatcher dispatcher;

static DeviceHandler stateMachineHandler(wifi, sensor, storage);
static DeviceStateMachine stateMachine(stateMachineHandler);

// Event loop
ESP_EVENT_DEFINE_BASE(HMI_EVENTS);
ESP_EVENT_DEFINE_BASE(SENSOR_EVENTS);

extern "C" void statemachine_main(void* args) {
    DeviceStateMachine* pSm = static_cast<DeviceStateMachine*>(args);

    while (true) {
        pSm->process();

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

extern "C" void hmi_main(void* args) {
    HmiManager* pHmi = static_cast<HmiManager*>(args);

    while (true) {
        pHmi->update();

        vTaskDelay(pdMS_TO_TICKS(50));
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
    dispatcher.registerEventHandler(HMI_EVENTS, ESP_EVENT_ANY_ID, hmi);
    dispatcher.registerEventHandler(SENSOR_EVENTS, DATA_ACQUIRED, hmi);

    // application tasks
    xTaskCreate(statemachine_main, "StateMachineTask", SM_STACK_SIZE, &stateMachine, uxTaskPriorityGet(NULL), NULL);
    xTaskCreate(hmi_main, "HmiTask", HMI_STACK_SIZE, &hmi, 4, NULL);
}


#include "esp_event.h"

#include "DeviceState/DeviceStateMachine.h"
#include "DeviceStorage/DeviceStorage.h"
#include "WiFi/WiFiManager.h"

ESP_EVENT_DEFINE_BASE(STATEMACHINE_EVENTS);

/**
 * This function is called when an state event is posted to queue.
 *  
 * @param handler_arg the handler argument (in this case the StataMachine)
 * @param base the base event id
 * @param id the specific event id
 * @param event_data the custom event data according specific event id
 */
static void app_state_main(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data) {
    DeviceStateMachine* pStateMachine = static_cast<DeviceStateMachine*>(handler_arg);
    pStateMachine->process();
}

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

    // The device state are managed by a user defined loop. In opposite to a simple while(true) { process(); }
    // this ensures on any idle state given hook method is called.
    esp_event_loop_handle_t stateLoopHandle;
    esp_event_loop_args_t loop_args = {
        .queue_size = 5,
        .task_name = "stateLoop",
        .task_priority = uxTaskPriorityGet(NULL), // use current main priority
        .task_stack_size = 2*1024, // 2kB
        .task_core_id = 0
    };
    
    esp_event_loop_create(&loop_args, &stateLoopHandle);
    esp_event_handler_instance_register_with(stateLoopHandle, STATEMACHINE_EVENTS, ESP_EVENT_ANY_ID, app_state_main, &stateMachine, NULL);

    // starts the state machine and therefore the entiry device
    stateMachine.start(stateLoopHandle);
}

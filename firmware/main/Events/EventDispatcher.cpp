#include "EventDispatcher.h"

#define event_STACK_SIZE configMINIMAL_STACK_SIZE

/**
 * Event handler callback for hmi events
 */
static void eventHandler(void* context, esp_event_base_t base, int32_t id, void* data) {
    EventHandlerIfc* pHandler = static_cast<EventHandlerIfc*>(context);
    pHandler->onEvent(base, id, data);
}

esp_event_loop_handle_t EventDispatcher::s_appLoopHandle;

EventDispatcher::EventDispatcher() {
}

void EventDispatcher::initialize() {
    if (s_appLoopHandle) {
        return;
    }

    esp_event_loop_create_default();
    esp_event_loop_args_t eventLoopArgs = {
        .queue_size = 10,
        .task_name = "eventLoop",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = event_STACK_SIZE,
        .task_core_id = tskNO_AFFINITY
    };
    esp_event_loop_create(&eventLoopArgs, &s_appLoopHandle);
}


void EventDispatcher::registerEventHandler(esp_event_base_t eventBase, int32_t eventId, EventHandlerIfc& handler) {
    if (s_appLoopHandle) {
        return;
    }

    esp_event_handler_instance_register_with(s_appLoopHandle, eventBase, eventId, eventHandler, &handler, NULL);
}
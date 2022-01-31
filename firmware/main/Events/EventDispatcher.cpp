#include "EventDispatcher.h"

#include "Events.h"
#include "esp_timer.h"

#define event_STACK_SIZE configMINIMAL_STACK_SIZE

// A local timer info struct to manage timers inside IDF callback function. 
struct TimerInfo {
public:
    TimerInfo(EventDispatcher& dispatcher, esp_timer_handle_t handle, TimerId id) :
        dispatcher(dispatcher),
        handle(handle),
        timerId(id) {}

    EventDispatcher& dispatcher;
    esp_timer_handle_t handle;
    TimerId timerId;
};

// Event Callback mapper
static void eventHandler(void* context, esp_event_base_t base, int32_t id, void* data) {
    EventHandlerIfc* pHandler = static_cast<EventHandlerIfc*>(context);
    pHandler->onEvent(base, id, data);
}

// Timer Once Callback mapper
static void timerHandlerOneShote(void* arg) {
    TimerInfo* pInfo = static_cast<TimerInfo*>(arg);
    pInfo->dispatcher.sendEvent(ESP_EVENT_ANY_BASE, TIMER_ELAPSED, &pInfo->timerId, sizeof(pInfo->timerId));
    // only delete oneshote timer
    esp_timer_delete(pInfo->handle);
}

esp_event_loop_handle_t EventDispatcher::s_appLoopHandle;
uint32_t EventDispatcher::s_timerIdx;

EventDispatcher::EventDispatcher() {
}

void EventDispatcher::initialize() {
    if (s_appLoopHandle) return;

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

void EventDispatcher::sendEvent(esp_event_base_t eventBase, int32_t eventId, void* data, size_t dataSize) {
    if (s_appLoopHandle) return;

    esp_event_post_to(s_appLoopHandle, eventBase, eventId, data, dataSize, 100);
}

void EventDispatcher::sendEventIsr(esp_event_base_t eventBase, int32_t eventId, void* data, size_t dataSize) {
    if (s_appLoopHandle) return;

    esp_event_isr_post_to(s_appLoopHandle, eventBase, eventId, data, dataSize, NULL);
}

TimerId EventDispatcher::startOneShotTimer(uint32_t durationMs) {
    // create a copy, to ensure other tasks do not override unique index
    TimerId idx = s_timerIdx++;

    esp_timer_create_args_t timerArgs = {
        .callback = timerHandlerOneShote,
        .arg = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = NULL,
        .skip_unhandled_events = false
    };
    esp_timer_handle_t timerHandler;
    esp_timer_create(&timerArgs, &timerHandler);

    sendEvent(ESP_EVENT_ANY_BASE, TIMER_STARTED, &idx, sizeof(idx));
    esp_timer_start_once(timerHandler, durationMs);

    return idx;
}

void EventDispatcher::registerEventHandler(esp_event_base_t eventBase, int32_t eventId, EventHandlerIfc& handler) {
    if (s_appLoopHandle) return;

    esp_event_handler_instance_register_with(s_appLoopHandle, eventBase, eventId, eventHandler, &handler, NULL);
}
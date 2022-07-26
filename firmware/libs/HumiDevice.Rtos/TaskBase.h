#ifndef TASK_BASE_H_
#define TASK_BASE_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/message_buffer.h>
#include <freertos/timers.h>

// Project includes
#include "TaskIfc.h"
#include "TimerServiceIfc.h"
#include "TimerEvent.h"
#include "EventRuntime.h"

/**
 * Wrapper for FreeRTOS Task.
 * 
 * It wrappes framework FreeRTOS Task into type-safe task. Tasks are an unit which encapsulate
 * the state and behavior from other tasks. It interconnects to other tasks with queues.
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/system/freertos.html#task-api
 * 
 * @tparam QUEUE_LENGTH The length of incoming queue. Is shared with all possible event types.
 * @tparam ITEM_SIZE The biggest event which can be received from this task. Attention: Set as small as possible to save memory
 */
template<int QUEUE_LENGTH, int ITEM_SIZE>
class TaskBase : public TaskIfc, public TimerServiceIfc {
public:
    /**
     * Constructor 
     */
    TaskBase():
        m_isInitialized(false),
        m_isStarted(false),
        m_isRunning(true),
        m_xQueueHandle(INVALID_SUBSCRIBER_ID) {
            static_assert(ITEM_SIZE < MAX_MESSAGE_SIZE, "MAX_MESSAGE_SIZE must be greater than any ITEM_SIZE");
    }

    /**
     * Destructor
     */
    virtual ~TaskBase() {};

    /**
     * @see TaskIfc::initialize()
     */
    void initialize(const TaskId taskId) { 
        if (m_isInitialized) ESP_LOGE("Application", "Task is already initialized. Do not call initialize() twice on same task");

        m_xTaskHandle = taskId;
        m_xQueueHandle = xMessageBufferCreateStatic(sizeof(m_xQueueStorage), m_xQueueStorage, &m_xQueueBuffer);

        onInitialize();
        m_isInitialized = true;
    }

    /**
     * @see TaskIfc::start()
     */
    void start() {
        if (!m_isInitialized) ESP_LOGE("Application", "Task is not initialized. Call initialize() before start this task");

        if (!m_isStarted) {
            onStart();
        }

        m_isStarted = true;
    }

    /**
     * @see EventSubscriberIfc::getSubscriberId()
     */
    virtual SubscriberId getSubscriberId() const override {
        return m_xQueueHandle;
    }
    
    /**
     * @see EventSubscriberIfc::getTaskId()
     */
    virtual TaskId getTaskId() const override {
        return m_xTaskHandle;
    }

    /**
     * @see EventSubscriberIfc::execute()
     */
    virtual void execute(EventId eventId, Deserializer* pEvent = NULL) override {
        if (eventId == TimerEventId) {
            TimerEvent timerEvent(*pEvent);
            onHandleTimer(timerEvent.getId());
        }
        else {
            onHandleTimer(eventId, pEvent);
        }
    }

    /**
     * @see TimerServiceIfc::startPeriodicTimer()
     */
    TimerId startPeriodicTimer(const uint32_t period) {
        return EventRuntime::startTimer(m_xQueueHandle, period, true);
    }

    /**
     * @see TimerServiceIfc::startOneShotTimer()
     */
    TimerId startOneShotTimer(const uint32_t delay) {
        return EventRuntime::startTimer(m_xQueueHandle, delay, false);
    }

protected:
    /**
     * Initialize the task
     */
    virtual void onInitialize() = 0;

    /**
     * Starts the task
     */
    virtual void onStart() = 0;

    /**
     * Handle event with and without data
     * 
     * @see EventSubscriberIfc::onExecute()
     * @see onHandleTimer
     */
    virtual void onHandleEvent(EventId eventId, Deserializer* pEvent = NULL) = 0;

    /**
     * Handle all timers
     * 
     * @see EventSubscriberIfc::onExecute()
     * @see onHandleEvent
     */
    virtual void onHandleTimer(const TimerId timerId) {
        // nothing to do
    }

private:
    // Members
    bool m_isInitialized;
    bool m_isStarted;
    bool m_isRunning;

    TaskId m_xTaskHandle;
    SubscriberId m_xQueueHandle;
    StaticMessageBuffer_t m_xQueueBuffer;
    uint8_t m_xQueueStorage[QUEUE_LENGTH * ITEM_SIZE + 4];
};


#endif // TASK_BASE_H_
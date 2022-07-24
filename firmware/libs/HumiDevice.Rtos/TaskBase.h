#ifndef TASK_BASE_H_
#define TASK_BASE_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/message_buffer.h>

// Project includes
#include "TaskIfc.h"

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
class TaskBase : public TaskIfc  {
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
        onExecute(eventId, pEvent);
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
     * @see EventSubscriberIfc::onExecute()
     */
    virtual void onExecute(EventId eventId, Deserializer* pEvent = NULL) = 0;

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
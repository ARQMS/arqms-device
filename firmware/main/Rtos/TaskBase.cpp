#include "TaskBase.h"


TaskBase::TaskBase() :
    m_isInitialized(false),
    m_isStarted(false) {
}

TaskBase::~TaskBase() {
}

void TaskBase::initialize() {
    if (!m_isInitialized) {
        onInitialize();
    }
}

void TaskBase::start() {
    if (!m_isInitialized) assert("Task is not initialized. Call initialize() before start this task");

    if (!m_isStarted) {
        onStart();
    }
}

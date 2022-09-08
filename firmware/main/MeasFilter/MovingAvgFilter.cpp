#include "MovingAvgFilter.h"

MovingAvgFilter::MovingAvgFilter() :
    m_avgVal(0.f),
    m_sum(0.f),
    m_count(0U) {
}

MovingAvgFilter::~MovingAvgFilter() {
}

void MovingAvgFilter::reset() {
    m_sum = 0.f;
    m_avgVal = 0.f;
    m_count = 0U;
}

size_t MovingAvgFilter::getCount() const {
    return m_count;
}

void MovingAvgFilter::update(const float32_t value) {
    m_sum += value;
    m_count++;

    m_avgVal = m_sum / m_count;
}

float32_t MovingAvgFilter::get() const {
    return m_avgVal;
}
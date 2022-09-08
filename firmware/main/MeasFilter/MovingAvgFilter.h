#ifndef MOVING_AVG_FILTER_H_
#define MOVING_AVG_FILTER_H_

// Platform includes
#include <HumiDevice.Platform/Platform.h>

/**
 * A moving average is commonly used with time series data to smooth out short-term fluctuations and highlight longer-term trends or cycles.
 * 
 * @see https://en.wikipedia.org/wiki/Moving_average
 */
class MovingAvgFilter {
public:
    /**
     * Constructor
     */
    MovingAvgFilter();

    /**
     * Destructor
     */
    ~MovingAvgFilter();

    /**
     * Resets the filter
     */
    void reset();

    /**
     * Gets the number of filtered samples
     * @return size_t number of data samples
     */
    size_t getCount() const;

    /**
     * Adds given value to this filter
     * 
     * @param value to filter
     */
    void update(const float32_t value);

    /**
     * Gets the current filtered value
     * 
     * @return float32_t filtered value
     */
    float32_t get() const;

private:
    float32_t m_avgVal;
    float32_t m_sum;
    size_t m_count;
};


#endif // MOVING_AVG_FILTER_H_
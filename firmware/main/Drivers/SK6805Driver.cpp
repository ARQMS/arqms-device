#include "SK6805Driver.h"

#include <freertos/FreeRTOS.h>
#include <esp_attr.h>

uint32_t SK6805Driver::T0hTicks = 0;
uint32_t SK6805Driver::T1hTicks = 0;
uint32_t SK6805Driver::T0lTicks = 0;
uint32_t SK6805Driver::T1lTicks = 0;  

/**
 * Conver RGB data to RMT format.
 * based on https://github.com/espressif/esp-idf/tree/v4.4.1/examples/peripherals/rmt/led_strip
 *
 * @note LED R,G,B each contains 256 different choices (i.e. uint8_t)
 *
 * @param[in] src: Pointer to the buffer storing the raw data that needs to be converted to rmt format. 
 * @param[in] dest: Pointer to the buffer storing the rmt format data. 
 * @param[in] srcSize: The raw data size. 
 * @param[in] wantedNum: The number of rmt format data that wanted to get. 
 * @param[out] translatedSize: The size of the raw data that has been converted to rmt format, 
 *                              it should return 0 if no data is converted in user callback. 
 * @param[out] itemNum: number of RMT items which are converted from source data
 */
extern "C" void IRAM_ATTR rmtAdapterCallback(const void* src, 
                                rmt_item32_t* dest, 
                                size_t srcSize, 
                                size_t wantedNum, 
                                size_t* translatedSize, 
                                size_t* itemNum) {
    if (src == NULL || dest == NULL) {
        *translatedSize = 0;
        *itemNum = 0;
        return;
    }  

    const rmt_item32_t bit0 = {{{ SK6805Driver::T0hTicks, 1, SK6805Driver::T0lTicks, 0 }}}; // Logical 0
    const rmt_item32_t bit1 = {{{ SK6805Driver::T1hTicks, 1, SK6805Driver::T1lTicks, 0 }}}; // Logical 1

    size_t size = 0;
    size_t num = 0;

    uint8_t *psrc = (uint8_t *)src;
    rmt_item32_t *pdest = dest;
    while (size < srcSize && num < wantedNum) {
        // convert each bit into rmt_item32_t
        for (int i = 0; i < 8; i++) {
            // MSB first
            if (*psrc & (1 << (7 - i))) {
                pdest->val =  bit1.val;
            } 
            else {
                pdest->val = bit0.val;
            }
            num++;
            pdest++;
        }
        size++;
        psrc++;
    }

    *translatedSize = size;
    *itemNum = num;
}

SK6805Driver::SK6805Driver(const uint8_t rmtChannel, const uint8_t gpio) : 
    m_info(),
    m_lastRefreshTick(0),
    m_isActive(true) {
    m_info.rmtChannel = (rmt_channel_t)rmtChannel;
    m_info.gpioPin = (gpio_num_t)gpio;
}

SK6805Driver::~SK6805Driver() {
    // nothing to do
}

void SK6805Driver::initialize() {
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(m_info.gpioPin, m_info.rmtChannel);
    config.clk_div = 2; // set counter to 40MHz
    rmt_config(&config);
    rmt_driver_install(config.channel, 0, 0);

    uint32_t counterClkHz = 0;
    rmt_get_counter_clock(m_info.rmtChannel, &counterClkHz);

    // ns -> ticks
    float32_t ratio = (float32_t)counterClkHz / 1e9f;
    T0hTicks = (uint32_t)(ratio * SK6805_T0H_NS);
    T1hTicks = (uint32_t)(ratio * SK6805_T0L_NS);
    T0lTicks = (uint32_t)(ratio * SK6805_T1H_NS);
    T1lTicks = (uint32_t)(ratio * SK6805_T1L_NS);

    rmt_translator_init(m_info.rmtChannel, rmtAdapterCallback);

    clear();
}

void SK6805Driver::refresh() {
    static uint8_t DISABLED_BUFFER [3] = {0, 0 , 0};
    static_assert(sizeof(m_info.colorBuffer) == sizeof(DISABLED_BUFFER));

    uint32_t currentTick = xTaskGetTickCount();
    m_info.tickCounter -= pdTICKS_TO_MS((currentTick - m_lastRefreshTick));
    m_lastRefreshTick = currentTick;
    if (m_info.tickCounter <= 0 && m_info.period > 0) {
        m_info.isOn = !m_info.isOn;
        m_info.tickCounter = m_info.period;
    } else if (m_info.period == 0) {
        m_info.isOn = true;
        m_info.tickCounter = 0;
    }

    uint8_t* pData = m_info.isOn && m_isActive ? m_info.colorBuffer : DISABLED_BUFFER;
    
    rmt_write_sample(m_info.rmtChannel, pData, sizeof(m_info.colorBuffer), true);
    rmt_wait_tx_done(m_info.rmtChannel, 40 / portTICK_PERIOD_MS);
}

void SK6805Driver::clear() {
    setColor({0, 0, 0}); // disable LED

    m_lastRefreshTick = xTaskGetTickCount();
    refresh();
}

void SK6805Driver::enable() {
    m_isActive = true;

    refresh();
}

void SK6805Driver::disable() {
    m_isActive = false;

    refresh();
}

void SK6805Driver::setColor(RGB color, const uint32_t period) {
    // according to Datasheet chapter 15
    m_info.colorBuffer[0] = color.green;
    m_info.colorBuffer[1] = color.red;
    m_info.colorBuffer[2] = color.blue;

    updatePeriod(period);
}

void SK6805Driver::updatePeriod(const uint32_t period) {
    m_info.period = period;
}
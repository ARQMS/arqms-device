#ifndef SK6805DRIVER_H_
#define SK6805DRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// idf includes
#include "driver/rmt.h"
#include "hal/gpio_types.h"

/**
 * RGB structure
 */
struct RGB {
    /**
     * Default constructor
     */
    explicit RGB() {
    };

    /**
     * Constructor
     * 
     * @param red color
     * @param green color
     * @param blue color
     */
    RGB(uint32_t red, uint32_t green, uint32_t blue) :
        green(green & 0xFF), 
        red(red & 0xFF), 
        blue(blue & 0xFF) {
    };

    union {
        struct {
            /**
             * Green color channel between 0.255
             */
            uint8_t green;

            /**
             * Red color channel between 0.255
             */
            uint8_t red;

            /**
             * blue color channel between 0.255
             */
            uint8_t blue;
        };
        uint32_t hex;
    };
};

/**
 * LED Driver for SK6805. This driver supports a single led
 * 
 * It re-uses the internal RMT (Remote control Transceiver) to generate any sequence pattern.
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/v4.4.1/esp32c3/api-reference/peripherals/rmt.html
 * @see https://www.mouser.ch/datasheet/2/737/Datasheet-2489789.pdf
 */
class SK6805Driver {
public:
    // According to Datasheet chapter 13 Timing waveform
    static const uint32_t SK6805_T0H_NS     = 300;
    static const uint32_t SK6805_T0L_NS     = 1000;
    static const uint32_t SK6805_T1H_NS     = 600;
    static const uint32_t SK6805_T1L_NS     = 300;
    static const uint32_t SK6805_RESET_NS   = 100000;

    /**
     * Constructor
     * 
     * @param rmtChannel the rmt channel to use
     * @param gpio the gpio pin number
     */
    SK6805Driver(const uint8_t rmtChannel, const uint8_t gpio);

    /**
     * Destructor
     */
    ~SK6805Driver();

    /**
     * Inirialize the driver
     */
    void initialize();

    /**
     * Refreshes the led according to internal info structure.
     * 
     * @note Depends on calling refresh the period is more accurate. It's 
     * recommended to call this at least with 25Hz.
     */
    void refresh();

    /**
     * Set the color of pixel
     * 
     * @param color the color 
     * @param period the blinking period in ms. The accuracy is based on refresh rate!
     */
    void setColor(RGB color, const uint32_t period = 0);

    /**
     * Sets the period of blinking. Without color change
     * 
     * @param period the blinking period in ms. The accuracy is based on refresh rate!
     */
    void updatePeriod(const uint32_t period);

    /**
     * Turns off led 
     */
    void clear();

    /**
     * Enable driver
     */
    void enable();

    /**
     * Disable disable
     */
    void disable();

    // Ticks configuration. Available after initialize() is called
    static uint32_t T0hTicks;
    static uint32_t T1hTicks;
    static uint32_t T0lTicks;
    static uint32_t T1lTicks;      

private:

    /**
     * The led information struct
     */
    struct LedInfo {
        /**
         * Period duration in ms for blinking led
         * 
         * Negative or 0 period indicates a disabled period and 
         * led is always on/off
         */
        uint32_t period;


        /**
         * Indicates the current state, on or off
         */
        bool isOn;

        // internal helper structure
        int32_t tickCounter;        //< ticks left until period is over
        uint8_t colorBuffer[3];     //< represents the color buffer
        rmt_channel_t rmtChannel;   //< the rmt channel
        gpio_num_t gpioPin;         //< the associated gpio hardware pin
    };

    LedInfo m_info;
    uint32_t m_lastRefreshTick;
    bool m_isActive;
};

#endif // SK6805DRIVER_H_

#ifndef GPIO_IRQ_HANDLER_H_
#define GPIO_IRQ_HANDLER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>
#include <HumiDevice.Rtos/EventPublisherSingle.h>

/**
 * GPIO interrupt handler which forwards interrupts to PushBtn task
 */
class GpioIrqHandler {
public:
    EventPublisherSingle PushBtn;

public:
    /**
     * Returns this push button instance
     * @return GpioIrqHandler& push button control instance
     */
    static GpioIrqHandler& getInstance();

    /**
     * IDF callback whenever a gpio interrupt is fired
     * 
     * @note This is called in ISR Context, so do not print any LOGS or call IDF methods which 
     * does not end with *FromISR() methods. It should forward event as fast as possible to
     * any task.
     * 
     * @param arg is a custom argument defined on registrator
     */
    static void onGpioIsr(void* arg);

private:
    /**
     * Constructor
     */
    GpioIrqHandler();

    /**
     * Destructor
     */
    ~GpioIrqHandler();

    static GpioIrqHandler s_pInstance;
};


#endif // GPIO_IRQ_HANDLER_H_
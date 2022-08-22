#ifndef ADC_H_
#define ADC_H_

// Platform Include
#include <HumiDevice.Platform/Platform.h>

#include "driver/gpio.h"
#include "driver/adc.h"

class ADC{
public:
    /**
     * Constructor
     * 
     * @param gpio the gpio pin number 
     */
    ADC();

    /**
     * Destructor
     */
    ~ADC();

    /**
     * Initialize ADC Peripheral
     */
    void initialize();

    void calibrate();

    int readADC();
};
#endif //ADC_H_
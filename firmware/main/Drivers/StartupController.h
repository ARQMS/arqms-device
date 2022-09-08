#ifndef STARTUP_CONTROLLER_H_ 
#define STARTUP_CONTROLLER_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "Control/Persistency/NvsStorageDriver.h"

/**
 * StartupController to initialize ESP32-C3 HumiDevice hardware. 
 */
class StartupController {
public:
    /**
     * initialize cpu
     */
    static void initialize();

    /**
     * initialize cpu
     */
    static void initializeGpio();

    /**
     * Initialize spi 
     */
    static void initializeSpi1();

    /**
     *  Initialize ADC
     */
    static void initializeAdc1();

    /**
     * Initialize gpio interrupts
     */
    static void initializeIrq();

private:
    
    /**
     * Private default constructor
     */
    StartupController(void);

    /**
     * Private default deconstructor
     */
    ~StartupController(void);

    /**
     * Provide the private copy constructor so the compiler does not generate the default one.
     */
    StartupController(const StartupController& other);

    /**
     * Provide the private assignment operator so the compiler does not generate the default one.
     */
    StartupController& operator=(const StartupController& other);

    // static members
    static NvsStorageDriver s_nvsDriver;
};


#endif // STARTUP_CONTROLLER_H_
#include <stdio.h>

#include "DeviceStateMachine.h"

static DeviceStateMachine stateMachine;

extern "C" void app_main(void) {
    // hw_initialize();
    // wakeup_initialize();
    // irq_initialize();


    while (true) {
        stateMachine.process();
    }
}

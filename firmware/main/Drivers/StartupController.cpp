#include "StartupController.h"

#include "Drivers/ApplicationHardwareConfig.h"
#include "Control/ControlTask.h"
#include "CloudLink/ConfigurationService/ConfigurationService.h"

NvsStorageDriver StartupController::s_nvsDriver;

void StartupController::initialize() {
    s_nvsDriver.initialize();

    ControlTask::setStorageDriver(&StartupController::s_nvsDriver);
    ConfigurationService::setStorageDriver(&StartupController::s_nvsDriver);
}

void StartupController::initializeGpio() {
    // Air Indicator GPIO
    gpio_reset_pin(LED_AIR_POOR);
    gpio_reset_pin(LED_AIR_MOD);
    gpio_reset_pin(LED_AIR_GOOD);

    gpio_set_direction(LED_AIR_POOR, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_AIR_MOD, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_AIR_GOOD, GPIO_MODE_OUTPUT);

    // Wlan Indicator GPIO
    // TODO

    // BME680 SPI GPIO
    // TODO

    // Usr Button GPIO  
    // TODO

    // VBat Monitor ADC GPIO
    // TODO

    // PSU State GPIO
    // TODO
}

void StartupController::initializeSpi1() {
    // TODO
}

void StartupController::initializeAdc1() {
    // TODO
}
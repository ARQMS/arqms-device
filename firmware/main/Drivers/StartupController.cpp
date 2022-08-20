#include "StartupController.h"

// Project includes
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
    gpio_reset_pin(LED_WLAN);
    gpio_set_direction(LED_WLAN, GPIO_MODE_OUTPUT);

    // BME680 SPI GPIO
    // nothing do to

    // Usr Button GPIO  
    // TODO

    // VBat Monitor ADC GPIO
    // TODO

    // PSU State GPIO
    // TODO
}

void StartupController::initializeSpi1() {
    // Datasheet [0] https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme680-ds001.pdf
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/peripherals/spi_slave.html
    spi_bus_config_t spiBusCfg = {
        .mosi_io_num     = static_cast<int32_t>(BME680_SPI_MOSI),
        .miso_io_num     = static_cast<int32_t>(BME680_SPI_MISO),
        .sclk_io_num     = static_cast<int32_t>(BME680_SPI_SCK),
        .data2_io_num    = -1,
        .data3_io_num    = -1,
        .data4_io_num    = -1,
        .data5_io_num    = -1,
        .data6_io_num    = -1,
        .data7_io_num    = -1,
    };
    spi_device_interface_config_t spiDevCfg = {
        .command_bits    = 0,
        .address_bits    = 0,
        .mode            = 3,                                       // Datasheet [0] page 40
        .clock_speed_hz  = SPI_MASTER_FREQ_10M,                     // Datasheet [0] page 3
        .spics_io_num    = static_cast<int32_t>(BME680_SPI_SS),
        .queue_size      = 7
    };
    spi_bus_initialize(BME680_SPI_BUS, &spiBusCfg, SPI_DMA_DISABLED);
    spi_bus_add_device(BME680_SPI_BUS, &spiDevCfg, &s_pSpi);
}

void StartupController::initializeAdc1() {
    // TODO
}
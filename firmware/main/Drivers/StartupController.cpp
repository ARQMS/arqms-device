#include "StartupController.h"

#include "Drivers/ApplicationHardwareConfig.h"
#include "Control/ControlTask.h"
#include "Control/PushBtn/GpioIrqHandler.h"
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
    // nothing to do

    // BME680 SPI GPIO
    // nothing to do

    // Usr Button GPIO  
    gpio_set_direction(USR_BTN, GPIO_MODE_INPUT);
    gpio_set_direction(PRG_SW_RST_BTN, GPIO_MODE_INPUT);

    // VBat Monitor ADC GPIO
    // TODO

    // PSU State GPIO
    // TODO
}

void StartupController::initializeSpi1() {
    // Datasheet [0] https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bme680-ds001.pdf
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/peripherals/spi_slave.html
    spi_bus_config_t spiBusCfg = spi_bus_config_t();
    spiBusCfg.mosi_io_num     = static_cast<int32_t>(BME680_SPI_MOSI);
    spiBusCfg.miso_io_num     = static_cast<int32_t>(BME680_SPI_MISO);
    spiBusCfg.sclk_io_num     = static_cast<int32_t>(BME680_SPI_SCK);
    spiBusCfg.data2_io_num    = -1;
    spiBusCfg.data3_io_num    = -1;
    spiBusCfg.data4_io_num    = -1;
    spiBusCfg.data5_io_num    = -1;
    spiBusCfg.data6_io_num    = -1;
    spiBusCfg.data7_io_num    = -1;
    
    spi_device_interface_config_t spiDevCfg = spi_device_interface_config_t();
    spiDevCfg.command_bits    = 0;
    spiDevCfg.address_bits    = 0;
    spiDevCfg.mode            = 0;                                       // Datasheet [0] page 40
    spiDevCfg.clock_speed_hz  = SPI_MASTER_FREQ_10M;                     // Datasheet [0] page 3
    spiDevCfg.spics_io_num    = static_cast<int32_t>(BME680_SPI_SS);
    spiDevCfg.queue_size      = 5;

    spi_bus_initialize(BME680_SPI_BUS, &spiBusCfg, SPI_DMA_DISABLED);
    spi_bus_add_device(BME680_SPI_BUS, &spiDevCfg, &s_pSpi);
}

void StartupController::initializeAdc1() {
    // TODO
}

void StartupController::initializeIrq() {
    gpio_config_t gpioConfig = gpio_config_t();
    gpioConfig.intr_type = GPIO_INTR_ANYEDGE;
    gpioConfig.pin_bit_mask = (1ULL << USR_BTN)
                            | (1ULL << PRG_SW_RST_BTN);
    gpioConfig.mode = GPIO_MODE_INPUT;
    gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&gpioConfig);

    // install gpio service
    gpio_install_isr_service(0);
    gpio_isr_handler_add(USR_BTN, &GpioIrqHandler::onGpioIsr, (void*)USR_BTN);
    gpio_isr_handler_add(PRG_SW_RST_BTN, &GpioIrqHandler::onGpioIsr, (void*)PRG_SW_RST_BTN);
}

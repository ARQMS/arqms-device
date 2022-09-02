#ifndef APPLICATION_HARDWARE_CONFIG_H_ 
#define APPLICATION_HARDWARE_CONFIG_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// IDF includes
#include "driver/gpio.h"
#include "driver/spi_master.h"

// Air indicator GPIO
static const gpio_num_t LED_AIR_POOR    = GPIO_NUM_21;
static const gpio_num_t LED_AIR_MOD     = GPIO_NUM_20;
static const gpio_num_t LED_AIR_GOOD    = GPIO_NUM_10;

// WLAN indicator GPIO
static const gpio_num_t LED_WLAN        = GPIO_NUM_2;

// BME680 GPIO
extern spi_device_handle_t s_pSpi;

#define BME680_SPI_BUS SPI2_HOST
#define SPI_MASTER_FREQ_1M      (APB_CLK_FREQ/80)
static const gpio_num_t BME680_SPI_SCK  = GPIO_NUM_4;
static const gpio_num_t BME680_SPI_SS   = GPIO_NUM_7;
static const gpio_num_t BME680_SPI_MOSI = GPIO_NUM_6;
static const gpio_num_t BME680_SPI_MISO = GPIO_NUM_5;

#endif // APPLICATION_HARDWARE_CONFIG_H_
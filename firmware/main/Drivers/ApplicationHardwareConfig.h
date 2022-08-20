#ifndef APPLICATION_HARDWARE_CONFIG_H_ 
#define APPLICATION_HARDWARE_CONFIG_H_

// Platform
#include <HumiDevice.Platform/Platform.h>

// IDF includes
#include "driver/gpio.h"

// Air indicator GPIO
static const gpio_num_t LED_AIR_POOR    = GPIO_NUM_21;
static const gpio_num_t LED_AIR_MOD     = GPIO_NUM_20;
static const gpio_num_t LED_AIR_GOOD    = GPIO_NUM_10;

// WLAN indicator GPIO
static const gpio_num_t LED_WLAN        = GPIO_NUM_2;

// Battery Status GPIO
static const gpio_num_t PCU_STAT_1      = GPIO_NUM_8;
static const gpio_num_t PCU_STAT_2      = GPIO_NUM_1;
static const gpio_num_t VBAT_MON        = GPIO_NUM_0;

#endif // APPLICATION_HARDWARE_CONFIG_H_
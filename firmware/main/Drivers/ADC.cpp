#include "ADC.h"

#include <freertos/FreeRTOS.h>
#include <esp_attr.h>

ADC::ADC(){

}

ADC::~ADC(){
    // nothing to do
}

void ADC::initialize(){
    // TODO: Initialize ADC1 as ADC2 is used by 
    //       the Wi-Fi Driver.
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_6db);
}

void ADC::calibrate(){
    // TODO: Calibrate ADC as Internal Voltage can vary between Devices       
}

int ADC::readADC(){
    int adc_value = adc1_get_raw(ADC1_CHANNEL_0);
    return adc_value;
}


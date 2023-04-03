/*
 * adc_init.h
 *
 *  Created on: Feb 15, 2023
 *      Author: Dell
 */

#ifndef MAIN_ADC_INCLUDE_ADC_INIT_H_
#define MAIN_ADC_INCLUDE_ADC_INIT_H_

#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_continuous.h"
#include "driver/gpio.h"


#define ADC_CONV_MODE       ADC_CONV_SINGLE_UNIT_1
#define ADC_OUTPUT_TYPE     ADC_DIGI_OUTPUT_FORMAT_TYPE1

void adc_read(uint16_t * light_adc, uint16_t * air_quality, uint16_t * soil_hum );
void continuous_adc_init(void);

#endif /* MAIN_ADC_INCLUDE_ADC_INIT_H_ */

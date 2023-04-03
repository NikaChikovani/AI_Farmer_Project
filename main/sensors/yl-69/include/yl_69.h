/*
 * yl_69.h
 *
 *  Created on: Feb 15, 2023
 *      Author: Dell
 */

#ifndef MAIN_SENSORS_YL_69_INCLUDE_YL_69_H_
#define MAIN_SENSORS_YL_69_INCLUDE_YL_69_H_

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

int Get_Soil_Humidity(int adc_val);



#endif /* MAIN_SENSORS_YL_69_INCLUDE_YL_69_H_ */

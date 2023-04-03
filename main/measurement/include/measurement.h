/*
 * measurement.h
 *
 *  Created on: Feb 15, 2023
 *      Author: Dell
 */

#ifndef MAIN_MEASUREMENT_INCLUDE_MEASUREMENT_H_
#define MAIN_MEASUREMENT_INCLUDE_MEASUREMENT_H_

#include "main.h"
#include "ds18b20.h"
#include "yl_69.h"
#include "adc_init.h"
#include "dht22.h"
#include "sdkconfig.h"
#include "esp32/rom/ets_sys.h"

#define MAX_DEVICES          1
#define DS18B20_RESOLUTION   (DS18B20_RESOLUTION_12_BIT)


typedef struct
{

	int soil_tem;
	int soil_hum;
	int air_temp;
	int air_hum;
	int air_quality;
	int light_int;

}sensor_paramtypedef;



void DHT_Init(void);
int DHT_read_data(void);

float Get_Air_Temp(void);
float Get_Air_Hum(void);

float Get_Soil_Temp(void);
int Soil_Temp_Sensor_Init(void);

void sensor_monitoring_task(void *pvParameters);





#endif /* MAIN_MEASUREMENT_INCLUDE_MEASUREMENT_H_ */

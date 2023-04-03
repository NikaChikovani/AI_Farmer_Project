
#include "measurement.h"

const char *TAG = "Measurement Task: ";
DeviceAddress tempSensors[1];
extern QueueHandle_t sensor_data_queue;

#define ADC_COEF  			1.1/4095
#define ADC_YL_COEF_YL  	3.3/4095

#define MQ135_COEF  10/0.04
#define YL_69_COEF  100/3.3
#define	LDR_COEF  	1000/0.8

int Soil_Temp_Sensor_Init(void){

	ds18b20_init(CONFIG_DS18B20_GPIO);

	return 1;
}

float Get_Soil_Temp(void){

	float cTemp = 0.0;
	ds18b20_requestTemperatures();
	cTemp = ds18b20_get_temp();

	return cTemp;

}

float Get_Soil_Hum(uint16_t  adc){

	float Hum = 0.0;
	float voltage = 0.0;

	voltage = (float)adc*ADC_YL_COEF_YL;

	if(voltage == 0){

		return 0;
	}

	Hum = (float)100 - (YL_69_COEF*voltage);

	if(Hum > 100){

		return 100;
	}else if(Hum < 0){

		return 0;
	}

	ESP_LOGI(TAG, "Volt : %0.2f", voltage);

	return Hum;

}

int Get_Air_Quality(uint16_t  adc){

	int quality = 0;
	float voltage = 0.0;

	voltage = (float)adc*ADC_COEF;
	quality = (int)(voltage*MQ135_COEF);

	ESP_LOGI(TAG, "Volt : %0.2f", voltage);


	return quality;

}

int Get_Light_Intensity(uint16_t  adc){

	int intensity = 0;
	float voltage = 0.0;

	voltage = (float)adc*ADC_COEF;
	intensity = (int)(voltage*LDR_COEF);

	ESP_LOGI(TAG, "Volt : %0.2f", voltage);

	return intensity;

}

void DHT_Init(void){

	 setDHTgpio(CONFIG_DHT22_GPIO);
}

int DHT_read_data(void){

	int ret = readDHT();
	errorHandler(ret);

	return ret;

}

float Get_Air_Temp(void){

	float temp;
	temp = getTemperature();

	return temp;
}

float Get_Air_Hum(void){

	float hum;
	hum = getHumidity();

	return hum;
}

void sensor_monitoring_task(void *pvParameters){

	sensor_paramtypedef sensors = {0};
	uint16_t ldr_adc = 0;
	uint16_t mq135_adc = 0;
	uint16_t yl_69_adc = 0;

	continuous_adc_init();


	 if (Soil_Temp_Sensor_Init() > 0 ){

		 ESP_LOGI(TAG," Soil Temp Sensor Init OK");

	 }

	 else {

		 ESP_LOGI(TAG," Soil Temp Sensor  FAIL");
	 }


	 DHT_Init();


	 ESP_LOGI(TAG," Air Temp-Hum Sensor Init OK");


	for(;;){


		adc_read(&ldr_adc, &mq135_adc, &yl_69_adc);

		sensors.soil_hum = (int)Get_Soil_Hum(yl_69_adc);

		sensors.air_quality = (int)Get_Air_Quality(mq135_adc);

		sensors.light_int = (int)Get_Light_Intensity(ldr_adc);

		DHT_read_data();

		sensors.soil_tem = (int)Get_Soil_Temp();

		sensors.air_hum = (int)Get_Air_Hum();

		sensors.air_temp = (int)Get_Air_Temp();


		ESP_LOGI(TAG,"Soil Temperature: %d\n", sensors.soil_tem);

		ESP_LOGI(TAG,"Air Temperature: %d\n", sensors.air_temp);

		ESP_LOGI(TAG,"Air Hum : %d\n", sensors.air_hum);

		ESP_LOGI(TAG,"Soil Humidity: %d\n", sensors.soil_hum);

		ESP_LOGI(TAG,"Air quality: %d\n", sensors.air_quality);

		ESP_LOGI(TAG,"Light intensity : %d\n", sensors.light_int);

		xQueueSend(sensor_data_queue, (void *)&sensors, (TickType_t)0);
		vTaskDelay(3000 / portTICK_PERIOD_MS);

	}



}

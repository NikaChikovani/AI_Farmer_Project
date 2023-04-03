/*
 * adc.c
 *
 *  Created on: Feb 15, 2023
 *      Author: Dell
 */

#include "adc_init.h"


#define EXAMPLE_READ_LEN   60   //12

static adc_channel_t channel[3] = {ADC_CHANNEL_3, ADC_CHANNEL_6, ADC_CHANNEL_7 }; // LDR, air_quality, soil_hum
int channel_num = 3;
static const char *TAG = "ADC INIT: ";
adc_continuous_handle_t adc_handle = NULL;

void continuous_adc_init(void)
{

	adc_continuous_handle_cfg_t adc_config = {
	    .max_store_buf_size = EXAMPLE_READ_LEN,
	    .conv_frame_size = EXAMPLE_READ_LEN,
	};
	ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};

    ESP_LOGI(TAG, "channel num is :%d",	channel_num);

    for (int i = 0; i < channel_num; i++) {

    		if(i == 2){

    				adc_pattern[i].atten = ADC_ATTEN_DB_11;  // ADC_ATTEN_DB_0

    		}else{
    				adc_pattern[i].atten = ADC_ATTEN_DB_0;  // ADC_ATTEN_DB_0
    			 }

            adc_pattern[i].channel = channel[i];
            adc_pattern[i].unit = ADC_UNIT_1;
            adc_pattern[i].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

            ESP_LOGI(TAG, "adc_pattern[%d].atten is :%x", i, adc_pattern[i].atten);
            ESP_LOGI(TAG, "adc_pattern[%d].channel is :%x", i, adc_pattern[i].channel);
            ESP_LOGI(TAG, "adc_pattern[%d].unit is :%x", i, adc_pattern[i].unit);
        }


	adc_continuous_config_t adc_io_cfg = {

	        .sample_freq_hz = 20 * 1000,
	        .conv_mode = ADC_CONV_MODE,
	        .format = ADC_OUTPUT_TYPE,
			.pattern_num = channel_num,
			.adc_pattern = adc_pattern
	    };

	    adc_continuous_config(adc_handle, &adc_io_cfg);

}


void adc_read(uint16_t * light_adc, uint16_t * air_quality, uint16_t * soil_hum )
{
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));

    esp_err_t ret;
    uint32_t ret_num = 0;
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    memset(result, 0, EXAMPLE_READ_LEN);

    uint32_t light_adc_sum = 0;
    uint32_t air_quality_sum = 0;
    uint32_t soil_hum_sum = 0;

    uint32_t light_adc_sum_cnt = 0;
    uint32_t air_quality_sum_cnt = 0;
    uint32_t soil_hum_sum_cnt = 0;

	*light_adc = 0;
	*air_quality = 0;
	*soil_hum = 0;

    ret = adc_continuous_read(adc_handle, result, EXAMPLE_READ_LEN, &ret_num, 1000);

    if (ret == ESP_OK) {
        ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32, ret, ret_num);
        for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
            adc_digi_output_data_t *p = (void*)&result[i];
            ESP_LOGI(TAG, "Unit: %d, Channel: %d, Value: %x", 1, p->type1.channel, p->type1.data);

            switch(p->type1.channel){

            	case ADC_CHANNEL_3:
            		light_adc_sum += p->type1.data;
            		light_adc_sum_cnt++;
            		break;

            	case ADC_CHANNEL_6:
            	    air_quality_sum += p->type1.data;
            	    air_quality_sum_cnt++;
            		break;

            	case ADC_CHANNEL_7:
            		soil_hum_sum += p->type1.data;
            		soil_hum_sum_cnt++;
            		break;

            }

        }


        *light_adc = (uint16_t)light_adc_sum/light_adc_sum_cnt;
        *air_quality = (uint16_t)air_quality_sum/air_quality_sum_cnt;
        *soil_hum = (uint16_t)soil_hum_sum/soil_hum_sum_cnt;


    }

    else {

            		ESP_LOGI(TAG,"Read Failed");

    }

    ESP_ERROR_CHECK(adc_continuous_stop(adc_handle));

}




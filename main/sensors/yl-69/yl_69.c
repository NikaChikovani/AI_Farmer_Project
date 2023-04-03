#include "yl_69.h"



int Get_Soil_Humidity(int adc_val){

	int hum = 0;

	if(adc_val < 1861){

		return 100;
	}


	hum = (int)( 100 - 100 *adc_val/4096);

	return hum;

}

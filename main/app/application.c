/*
 * application.c
 *
 *  Created on: Mar 22, 2023
 *      Author: Dell
 */

#include "application.h"

static const char *TAG = "APP:  ";
extern QueueHandle_t sensor_data_queue;
extern EventGroupHandle_t s_wifi_event_group;

bool irr_status = false;
bool light_status = false;

void light_turn_on(void);
void light_turn_off(void);
void irrigation_turn_on(void);
void irrigation_turn_off(void);
void json_payload_parse(char *buf);
void json_payload_create(sensor_paramtypedef *sensors, char *buf);

/*
char * post_data = "{" \
	"\"device_id\": \"greenhousetest12\"," 	 \
	"\"soil_temperature\": 12,"  	  \
	"\"soil_humidity\": 60,"	 	\
	"\"air_temperature\": 25,"		\
	"\"air_humidity\": 60,"    		\
	"\"air_quality\": 80,"			\
	"\"light_intensity\": 10,"  	\
	"\"irrigation_status\": true,"  \
	"\"lighting_status\": true"  	\
"}"; */

void GPIO_Init(void)
{

	gpio_set_direction(CONFIG_WIFI_LED_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_direction(CONFIG_IRRIGATION_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_direction(CONFIG_LIGHT_GPIO, GPIO_MODE_OUTPUT);

	gpio_set_level(CONFIG_WIFI_LED_GPIO, 0);
	gpio_set_level(CONFIG_IRRIGATION_GPIO, 0);
	gpio_set_level(CONFIG_LIGHT_GPIO, 0);


}

void light_turn_on(void)
{
	light_status = true;
	gpio_set_level(CONFIG_LIGHT_GPIO, 1);
	ESP_LOGE(TAG, "Turn on Light");
}

void light_turn_off(void)
{
	light_status = false;
	gpio_set_level(CONFIG_LIGHT_GPIO, 0);
	ESP_LOGE(TAG, "Turn off Light");
}

void irrigation_turn_on(void)
{
	irr_status = true;
	gpio_set_level(CONFIG_IRRIGATION_GPIO, 1);

	ESP_LOGE(TAG, "Turn on irrigation");
}

void irrigation_turn_off(void)
{
	irr_status = false;
	gpio_set_level(CONFIG_IRRIGATION_GPIO, 0);
	ESP_LOGE(TAG, "Turn off irrigation");
}

void json_payload_parse(char *buf)
{
	 cJSON *device_id = NULL;
	 cJSON *irrigation_status = NULL;
	 cJSON *lighting_status = NULL;

	 cJSON *json_payload = cJSON_Parse(buf);

	 if (json_payload == NULL)
	  {
	    	ESP_LOGE(TAG, "Json Parse Failed");

	    	return;

	  }

	 device_id = cJSON_GetObjectItemCaseSensitive(json_payload , "device_id");
	 irrigation_status = cJSON_GetObjectItemCaseSensitive(json_payload , "irrigation_status");
	 lighting_status = cJSON_GetObjectItemCaseSensitive(json_payload , "lighting_status");

	 if (device_id == NULL || irrigation_status == NULL || lighting_status == NULL)
	  {
		 cJSON_Delete(json_payload);
	     ESP_LOGE(TAG, "Json Parse Failed");

	     return;

	  }

	 if(strstr(device_id->valuestring, CONFIG_DEVICE_ID ) == NULL)
	 {
		 cJSON_Delete(json_payload);
	     ESP_LOGE(TAG, "Wrong Device ID");

	     return;

	 }

	 if(irrigation_status->valueint == true)
	 {
		 irrigation_turn_on();

	 }	 else{

		 irrigation_turn_off();
	 }

	 if(lighting_status->valueint == true)
	 {
		 light_turn_on();

	 }	 else{

		 light_turn_off();
	 }


	cJSON_Delete(json_payload);

}

void json_payload_create(sensor_paramtypedef *sensors, char *buf)
{
	cJSON *device_id = NULL;
	cJSON *irrigation_status = NULL;
	cJSON *lighting_status = NULL;
	cJSON *air_temp = NULL;
	cJSON *air_hum = NULL;
	cJSON *soil_temp = NULL;
	cJSON *soil_hum = NULL;
	cJSON *air_quality = NULL;
	cJSON *light_inten = NULL;

	char *ptr = NULL;

	memset(buf, 0, JSON_MSG_MAX_SIZE);

    cJSON *json_payload = cJSON_CreateObject();

    if (json_payload == NULL)
    {
    	ESP_LOGE(TAG, "Json Create Failed");
        return;
    }

    device_id = cJSON_CreateString(CONFIG_DEVICE_ID);
    if (device_id == NULL)
    {
    	cJSON_Delete(json_payload);
    	ESP_LOGE(TAG, "Json Create Failed");
        return;
    }

    air_temp = cJSON_CreateNumber(sensors->air_temp);
	air_hum = cJSON_CreateNumber(sensors->air_hum);
	soil_temp = cJSON_CreateNumber(sensors->soil_tem);
	soil_hum = cJSON_CreateNumber(sensors->soil_hum);
	air_quality = cJSON_CreateNumber(sensors->air_quality);
	light_inten = cJSON_CreateNumber(sensors->light_int);

	if(irr_status == true){

		irrigation_status = cJSON_CreateBool(true);

	}else {

		irrigation_status = cJSON_CreateBool(false);
	}

	if(light_status == true){

		lighting_status = cJSON_CreateBool(true);

	}else {

		lighting_status = cJSON_CreateBool(false);
	}

    cJSON_AddItemToObject(json_payload, "device_id", device_id);

    cJSON_AddItemToObject(json_payload, "soil_temperature", soil_temp);
    cJSON_AddItemToObject(json_payload, "soil_humidity", soil_hum);
    cJSON_AddItemToObject(json_payload, "air_temperature", air_temp);
    cJSON_AddItemToObject(json_payload, "air_humidity", air_hum);
    cJSON_AddItemToObject(json_payload, "air_quality", air_quality);
    cJSON_AddItemToObject(json_payload, "light_intensity", light_inten);

    cJSON_AddItemToObject(json_payload, "irrigation_status", irrigation_status);
    cJSON_AddItemToObject(json_payload, "lighting_status", lighting_status);

    ptr = cJSON_Print(json_payload);

    strcpy(buf, ptr);

    cJSON_Delete(json_payload);
    cJSON_free(ptr);

}

void wifi_monitoring_task(void *pvParameters)
{


	while (1)
	{



	    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
	            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT | WIFI_DISCONNECT_BIT,
	            pdFALSE,
	            pdFALSE,
	            portMAX_DELAY);

	      if (bits & WIFI_CONNECTED_BIT) {

	    	gpio_set_level(CONFIG_WIFI_LED_GPIO, 1);


	       } else if (bits & WIFI_DISCONNECT_BIT) {

	    	   gpio_set_level(CONFIG_WIFI_LED_GPIO, 0);
	    	   vTaskDelay(300 / portTICK_PERIOD_MS);
	    	   gpio_set_level(CONFIG_WIFI_LED_GPIO, 1);


	       }
	       else  gpio_set_level(CONFIG_WIFI_LED_GPIO, 0);

		vTaskDelay(50 / portTICK_PERIOD_MS);


	}

}


void main_task(void *pvParameters)
{

    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    char post_data[JSON_MSG_MAX_SIZE] = {0};
    esp_err_t err;
    sensor_paramtypedef sensors;

    char get_url[128] = {0};

    strcpy(get_url,CONFIG_GET_URL);
    strcat(get_url,CONFIG_DEVICE_ID);

    esp_http_client_config_t config = {
    	.url = CONFIG_POST_URL,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,
        .disable_auto_redirect = true,
		.keep_alive_enable = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);


    while(1){

        // POST
    	if(xQueueReceive(sensor_data_queue, (void *)&sensors, (TickType_t)0))
    	{
    	        esp_http_client_set_url(client, CONFIG_POST_URL);
    	        esp_http_client_set_method(client, HTTP_METHOD_POST);
    	        esp_http_client_set_header(client, "Content-Type", "application/json");

    	        json_payload_create(&sensors, post_data);

    	        printf(" %s \n", post_data);

    	        esp_http_client_set_post_field(client, post_data, strlen(post_data));
    	        err = esp_http_client_perform(client);
    	        if (err == ESP_OK)
    	        {
    	            ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRIu64,
    	                    esp_http_client_get_status_code(client),
    	                    esp_http_client_get_content_length(client));

    	            ESP_LOGI(TAG,"%s", local_response_buffer);
    	            memset(local_response_buffer, 0, MAX_HTTP_OUTPUT_BUFFER);

    	        } else {
    	        			ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    	        			esp_http_client_cleanup(client);
    	        			client = esp_http_client_init(&config);

    	        	   }

    	}

    	 // GET
    	esp_http_client_set_url(client, get_url);
    	esp_http_client_set_method(client, HTTP_METHOD_GET);
    	err = esp_http_client_perform(client);
    	if (err == ESP_OK) {
    	   ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRIu64,
    	   esp_http_client_get_status_code(client),
    	   esp_http_client_get_content_length(client));

    	   ESP_LOGI(TAG,"%s", local_response_buffer);
    	   json_payload_parse(local_response_buffer);
    	   memset(local_response_buffer, 0, MAX_HTTP_OUTPUT_BUFFER);

    	  } else {
    	        	ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    	        	esp_http_client_cleanup(client);
    	        	client = esp_http_client_init(&config);
    	    	 }


    	vTaskDelay(500 / portTICK_PERIOD_MS);


    }

}

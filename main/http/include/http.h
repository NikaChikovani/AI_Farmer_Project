/*
 * http.h
 *
 *  Created on: Feb 2, 2023
 *      Author: Dell
 */

#ifndef MAIN_INC_HTTP_H_
#define MAIN_INC_HTTP_H_


#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include <sys/param.h>


#define MAX_HTTP_OUTPUT_BUFFER 	 2048

esp_err_t _http_event_handler(esp_http_client_event_t *evt);

#endif /* MAIN_INC_HTTP_H_ */

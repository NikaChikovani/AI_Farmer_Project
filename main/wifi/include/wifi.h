/*
 * wifi.h
 *
 *  Created on: Feb 1, 2023
 *      Author: Dell
 */

#ifndef MAIN_INC_WIFI_H_
#define MAIN_INC_WIFI_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"


/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT 		BIT0
#define WIFI_FAIL_BIT      		BIT1
#define WIFI_DISCONNECT_BIT     BIT2


#define EXAMPLE_ESP_MAXIMUM_RETRY			5
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD 	WIFI_AUTH_OPEN


void wifi_init_sta(void);


#endif /* MAIN_INC_WIFI_H_ */

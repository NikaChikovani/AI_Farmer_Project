/*
 * main.h
 *
 *  Created on: Jan 26, 2023
 *      Author: Dell
 */

#ifndef MAIN_INC_MAIN_H_
#define MAIN_INC_MAIN_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "cJSON.h"
#include "sdkconfig.h"

#include "wifi.h"
#include "http.h"
#include "measurement.h"
#include "application.h"


#define	app_cpu				1
#define	pro_cpu				0

#define	JSON_MSG_MAX_SIZE		1024


#endif /* MAIN_INC_MAIN_H_ */

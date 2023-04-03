/*
 * application.h
 *
 *  Created on: Mar 22, 2023
 *      Author: Dell
 */

#ifndef MAIN_APP_APPLICATION_H_
#define MAIN_APP_APPLICATION_H_


#include "main.h"

void wifi_monitoring_task(void *pvParameters);
void main_task(void *pvParameters);
void GPIO_Init(void);


#endif /* MAIN_APP_APPLICATION_H_ */

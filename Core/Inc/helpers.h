/*
 * helpers.h
 *
 *  Created on: Feb 20, 2024
 *      Author: gnavi
 */

#ifndef INC_HELPERS_H_
#define INC_HELPERS_H_

#define LED1 GPIO_PIN_4
#define LED2 GPIO_PIN_5


#include <cmath>
#include <string.h>
#include "main.h"
#include "LIS3MDL.hpp"

void calculate_heading(float calib[], int *heading, int16_t z);
void handle_leds(int heading);
void send_heading_uart(int heading, UART_HandleTypeDef huart);
void handle_config_callback(UART_HandleTypeDef huart, ConfigHandler chandler, uint8_t cdata[],
							SPI_HandleTypeDef hspi);

#endif /* INC_HELPERS_H_ */

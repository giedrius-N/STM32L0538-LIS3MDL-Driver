/*
 * helpers.cpp
 *
 *  Created on: Feb 20, 2024
 *      Author: gnavi
 */
#include "helpers.h"

void calculate_heading(float calib[], int *heading, int16_t z) {
	int cal_heading = round(atan2(calib[1], calib[0]) * 180 / M_PI);

	if (z > 0) {
		cal_heading *= -1;
	}

	cal_heading += MAG_DECLINATION;
	if (cal_heading < 0)
		cal_heading += 360;
	else if (cal_heading > 360)
		cal_heading -= 360;

	*heading = cal_heading;
}

void handle_leds(int heading) {
	  if (heading >= 225 && 315 >= heading) {
		  HAL_GPIO_WritePin(GPIOB, LED1, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, LED2, GPIO_PIN_SET);

	  }
	  else if (heading >= 45 && 135 >= heading) {
		  HAL_GPIO_WritePin(GPIOA, LED2, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, LED1, GPIO_PIN_SET);
	  }
	  else {
		  HAL_GPIO_WritePin(GPIOA, LED2, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, LED1, GPIO_PIN_RESET);
	  }
}

void send_heading_uart(int heading, UART_HandleTypeDef huart) {
	char axisDataStr[20];
	sprintf(axisDataStr, "%d\r\n", heading);

	uint8_t axisData[20];
	memcpy(axisData, axisDataStr, strlen(axisDataStr));

	HAL_UART_Transmit(&huart, axisData, strlen(axisDataStr), HAL_MAX_DELAY);
	HAL_Delay(1000);
}

void handle_config_callback(UART_HandleTypeDef huart, ConfigHandler chandler, uint8_t cdata[],
																	SPI_HandleTypeDef hspi) {
	chandler.process_config_req(cdata, hspi);

	uint8_t config_callback[6];
	handle_config_callback(chandler, config_callback);

	HAL_UART_Transmit(&huart, config_callback, sizeof(config_callback), HAL_MAX_DELAY);
}




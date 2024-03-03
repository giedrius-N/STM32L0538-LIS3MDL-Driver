/*
 * helpers.cpp
 *
 *  Created on: Feb 20, 2024
 *      Author: gnavi
 */
#include "helpers.h"

void calibrate(MagnetometerRawData data, float *calib)
{
	// Converting raw data in gauss to teslas, substracting the bias offset
	float x_off = data.get_x() * SCALE_FACTOR_X - BIAS_X;
	float y_off = data.get_y() * SCALE_FACTOR_Y - BIAS_Y;
	float z_off = data.get_z() * SCALE_FACTOR_Z - BIAS_Z;

	// Soft Iron calibration
	calib[0] = SOFT_IRON_MATRIX_X[0] * x_off + SOFT_IRON_MATRIX_X[1] * y_off + SOFT_IRON_MATRIX_X[2] * z_off;
	calib[1] = SOFT_IRON_MATRIX_Y[0] * x_off + SOFT_IRON_MATRIX_Y[1] * y_off + SOFT_IRON_MATRIX_Y[2] * z_off;
	calib[2] = SOFT_IRON_MATRIX_Z[0] * x_off + SOFT_IRON_MATRIX_Z[1] * y_off + SOFT_IRON_MATRIX_Z[2] * z_off;
}

void calculate_heading(float *calib, int *heading, int16_t z) {
	int cal_heading = round(atan2(calib[1], calib[0]) * HALF_CIRCLE_DEGREES / M_PI);

	if (z > ZERO_VALUE) {
		cal_heading *= -1;
	}

	cal_heading += MAG_DECLINATION;
	if (cal_heading < ZERO_VALUE)
		cal_heading += FULL_CIRCLE_DEGREES;
	else if (cal_heading > FULL_CIRCLE_DEGREES)
		cal_heading -= FULL_CIRCLE_DEGREES;

	*heading = cal_heading;
}

void handle_leds(int heading) {
	  if (heading >= ANGLE_225 && ANGLE_315 >= heading) {
		  HAL_GPIO_WritePin(GPIOB, LED1, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, LED2, GPIO_PIN_SET);

	  }
	  else if (heading >= ANGLE_45 && ANGLE_135 >= heading) {
		  HAL_GPIO_WritePin(GPIOA, LED2, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, LED1, GPIO_PIN_SET);
	  }
	  else {
		  HAL_GPIO_WritePin(GPIOA, LED2, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, LED1, GPIO_PIN_RESET);
	  }
}

void send_heading_uart(int heading, UART_HandleTypeDef huart) {
	char axisDataStr[MAX_HEADING_SIZE];
	sprintf(axisDataStr, "%d\r\n", heading);

	uint8_t axisData[MAX_HEADING_SIZE];
	memcpy(axisData, axisDataStr, strlen(axisDataStr));

	HAL_UART_Transmit(&huart, axisData, strlen(axisDataStr), HAL_MAX_DELAY);
	HAL_Delay(ONE_SECOND);
}

void create_response(ConfigHandler chandler, uint8_t *result) {

	power_mode_response(chandler.get_power_mode(), result);
	temp_response(chandler.get_temp(), result);
	blocking_mode_response(chandler.get_blocking_mode(), result);

	result[4] = '\r';
	result[5] = '\n';
}

void power_mode_response(PowerMode power_mode, uint8_t *result) {
	switch(power_mode) {
		case PowerMode::LOW:
			result[0] = 'L';
			result[1] = 'P';
			break;
		case PowerMode::MEDIUM:
			result[0] = 'M';
			result[1] = 'P';
			break;
		case PowerMode::HIGH:
			result[0] = 'H';
			result[1] = 'P';
			break;
		case PowerMode::ULTRA_HIGH:
			result[0] = 'U';
			result[1] = 'H';
			break;
		default:
			result[0] = 'X';
			result[1] = 'X';
		}
}

void temp_response(TempMode temp, uint8_t *result) {
	switch(temp) {
		case TempMode::TEMPON:
			result[2] = 'T';
			break;
		case TempMode::TEMPOFF:
			result[2] = 'F';
			break;
		default:
			result[2] = 'X';
	}
}

void blocking_mode_response(BlockingMode blocking_mode, uint8_t *result) {
	switch(blocking_mode) {
		case BlockingMode::BLOCKING:
			result[3] = 'B';
			break;
		case BlockingMode::NONBLOCKING:
			result[3] = 'N';
			break;
		default:
			result[3] = 'X';
	}
}


void handle_config_callback(UART_HandleTypeDef huart, ConfigHandler chandler, uint8_t *cdata, SPI_HandleTypeDef hspi) {
	chandler.process_config_req(cdata, hspi);

	uint8_t config_callback[CONFIG_CALLBACK_SIZE];
	create_response(chandler, config_callback);

	HAL_UART_Transmit(&huart, config_callback, sizeof(config_callback), HAL_MAX_DELAY);
}




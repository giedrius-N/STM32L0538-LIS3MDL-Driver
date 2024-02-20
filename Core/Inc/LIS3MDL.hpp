/*
 * LIS3MDL.hpp
 *
 *  Created on: Feb 1, 2024
 *      Author: Giedrius Navickas
 */

#ifndef INC_LIS3MDL_HPP_
#define INC_LIS3MDL_HPP_

#include <cmath>
#include <stdio.h>

/*
 * DEFINES
 */
#define LIS3MDL_CS_GPIO_Port GPIOB
#define LIS3MDL_CS_Pin GPIO_PIN_12

#define ZERO_VALUE 0x00
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

#define BIAS_X 67020.821058
#define BIAS_Y -93341.600766
#define BIAS_Z -7830.178850
#define MAG_DECLINATION 5  //Magnetic declination in Czech Republic

const float SOFT_IRON_MATRIX_X[] = {0.359843, 0.025408, -0.043194};
const float SOFT_IRON_MATRIX_Y[] = {0.025408, 0.422331, -0.008441};
const float SOFT_IRON_MATRIX_Z[] = {-0.043194, -0.008441, 0.370303};

/*
 * Class
 */
class MagnetometerRawData {
	private:
		int16_t x;
		int16_t y;
		int16_t z;

	public:
		MagnetometerRawData();

		// Setters / getters
		void set_x(int16_t x) {
			this->x = x;
		}
		int16_t get_x(void) {
			return x;
		}

		void set_y(int16_t y) {
			this->y = y;
		}
		int16_t get_y(void) {
			return y;
		}

		void set_z(int16_t z) {
			this->z = z;
		}
		int16_t get_z(void) {
			return z;
		}

		void get_axis_data(SPI_HandleTypeDef hspi);
};



/*
 * Functions
 */
void LIS3MDL_Startup(SPI_HandleTypeDef hspi);
void LIS3MDL_SetDefault(SPI_HandleTypeDef hspi);

uint8_t LIS3MDL_ReadRegister(uint8_t regAddr, SPI_HandleTypeDef hspi);
void LIS3MDL_WriteRegister(uint8_t regAddr, uint8_t data, SPI_HandleTypeDef hspi);

int16_t LIS3MDL_GetXaxisData(SPI_HandleTypeDef hspi);
int16_t LIS3MDL_GetYaxisData(SPI_HandleTypeDef hspi);
int16_t LIS3MDL_GetZaxisData(SPI_HandleTypeDef hspi);

int16_t TwosCompToDec(uint16_t value);

void Calibrate(MagnetometerRawData data, float *calib);


#endif /* INC_LIS3MDL_HPP_ */

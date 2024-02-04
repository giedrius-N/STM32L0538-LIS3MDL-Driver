/*
 * LIS3MDL.hpp
 *
 *  Created on: Feb 1, 2024
 *      Author: Giedrius Navickas
 */

#ifndef INC_LIS3MDL_HPP_
#define INC_LIS3MDL_HPP_

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
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

/*
 * Functions
 */
void LIS3MDL_Startup(SPI_HandleTypeDef hspi);
void LIS3MDL_SetDefault(SPI_HandleTypeDef hspi);

uint8_t LIS3MDL_ReadRegister(uint8_t regAddr, SPI_HandleTypeDef hspi);
void LIS3MDL_WriteRegister(uint8_t regAddr, uint8_t data, SPI_HandleTypeDef hspi);

uint16_t LIS3MDL_GetXaxisData(SPI_HandleTypeDef hspi);
uint16_t LIS3MDL_GetYaxisData(SPI_HandleTypeDef hspi);
uint16_t LIS3MDL_GetZaxisData(SPI_HandleTypeDef hspi);

int16_t TwosCompToDec(uint16_t value);


#endif /* INC_LIS3MDL_HPP_ */

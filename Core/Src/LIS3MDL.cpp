#include "main.h"
#include "lis3mdl.hpp"


uint8_t LIS3MDL_ReadRegister(uint8_t regAddr, SPI_HandleTypeDef hspi)
{
	uint8_t data;

	//Select the device
	HAL_GPIO_WritePin(LIS3MDL_CS_GPIO_Port, LIS3MDL_CS_Pin, GPIO_PIN_RESET);

	//Set to MSB to READ bit
	uint8_t operation = (1 << 7) | regAddr;
	//Send transmit
	HAL_SPI_Transmit(&hspi, &operation, 1, HAL_MAX_DELAY);

	//Receive data from LIS3MDL
	HAL_SPI_Receive(&hspi, &data, 1, HAL_MAX_DELAY);

	//Deselect the device
	HAL_GPIO_WritePin(LIS3MDL_CS_GPIO_Port, LIS3MDL_CS_Pin, GPIO_PIN_SET);

	return data;
}

void LIS3MDL_WriteRegister(uint8_t regAddr, uint8_t data, SPI_HandleTypeDef hspi)
{
	HAL_GPIO_WritePin(LIS3MDL_CS_GPIO_Port, LIS3MDL_CS_Pin, GPIO_PIN_RESET);

	//Set MSB to WRITE bit
	uint8_t operation = regAddr & ~(1 << 7);

	// Send transmit
	HAL_SPI_Transmit(&hspi, &operation, 1, HAL_MAX_DELAY);

	// Send data to write
	HAL_SPI_Transmit(&hspi, &data, 1, HAL_MAX_DELAY);

	//Deselect the device
    HAL_GPIO_WritePin(LIS3MDL_CS_GPIO_Port, LIS3MDL_CS_Pin, GPIO_PIN_SET);
}


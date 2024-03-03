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

int16_t LIS3MDL_GetXaxisData(SPI_HandleTypeDef hspi)
{
	uint16_t a_axis_data;

	uint8_t x_data_L = LIS3MDL_ReadRegister(OUT_X_L, hspi);
	uint8_t x_data_H = LIS3MDL_ReadRegister(OUT_X_H, hspi);

	a_axis_data = (x_data_H << 8) | x_data_L;

	return twos_comp_to_dec(a_axis_data);
}

int16_t LIS3MDL_GetYaxisData(SPI_HandleTypeDef hspi)
{
	uint16_t y_axis_data;

	uint8_t y_data_L = LIS3MDL_ReadRegister(OUT_Y_L, hspi);
	uint8_t y_data_H = LIS3MDL_ReadRegister(OUT_Y_H, hspi);

	y_axis_data = (y_data_H << 8) | y_data_L;

	return twos_comp_to_dec(y_axis_data);
}

int16_t LIS3MDL_GetZaxisData(SPI_HandleTypeDef hspi)
{
	uint16_t z_axis_data;

	uint8_t z_data_L = LIS3MDL_ReadRegister(OUT_Z_L, hspi);
	uint8_t z_data_H = LIS3MDL_ReadRegister(OUT_Z_H, hspi);

	z_axis_data = (z_data_H << 8) | z_data_L;

	return twos_comp_to_dec(z_axis_data);
}

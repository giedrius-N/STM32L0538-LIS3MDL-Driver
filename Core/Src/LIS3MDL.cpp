#include "LIS3MDL.hpp"

void LIS3MDL_Startup(SPI_HandleTypeDef hspi)
{
	//Set full scale to +- 12 Hz
	uint8_t ctrlReg2 = 0x40;
	LIS3MDL_WriteRegister(CTRL_REG2, ctrlReg2, hspi);

	//Set up UHP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
	uint8_t ctrlReg1 = 0xFC;
	LIS3MDL_WriteRegister(CTRL_REG1, ctrlReg1, hspi);

	//Set up UHP mode on the Z axis
	uint8_t ctrlReg4 = 0x0C;
	LIS3MDL_WriteRegister(CTRL_REG4, ctrlReg4, hspi);

	//Set continuous measurement mode
	LIS3MDL_WriteRegister(CTRL_REG3, ZERO_VALUE, hspi);

	//Turn on Block data update
	uint8_t ctrlReg5 = 0xC0;
	LIS3MDL_WriteRegister(CTRL_REG5, ctrlReg5, hspi);

}

void LIS3MDL_SetDefault(SPI_HandleTypeDef hspi)
{
	//Reset to default
	LIS3MDL_WriteRegister(CTRL_REG2, ZERO_VALUE, hspi);

	uint8_t ctrlReg1 = 0x10;
	LIS3MDL_WriteRegister(CTRL_REG1, ctrlReg1, hspi);

	LIS3MDL_WriteRegister(CTRL_REG4, ZERO_VALUE, hspi);

	uint8_t ctrlReg3 = 0x03;
	LIS3MDL_WriteRegister(CTRL_REG3, ctrlReg3, hspi);

	LIS3MDL_WriteRegister(CTRL_REG5, ZERO_VALUE, hspi);
}

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
	uint16_t xAxisData;

	uint8_t xDataL = LIS3MDL_ReadRegister(OUT_X_L, hspi);
	uint8_t xDataH = LIS3MDL_ReadRegister(OUT_X_H, hspi);

	xAxisData = (xDataH << 8) | xDataL;

	return TwosCompToDec(xAxisData);
}

int16_t LIS3MDL_GetYaxisData(SPI_HandleTypeDef hspi)
{
	uint16_t yAxisData;

	uint8_t yDataL = LIS3MDL_ReadRegister(OUT_Y_L, hspi);
	uint8_t yDataH = LIS3MDL_ReadRegister(OUT_Y_H, hspi);

	yAxisData = (yDataH << 8) | yDataL;

	return TwosCompToDec(yAxisData);
}

int16_t LIS3MDL_GetZaxisData(SPI_HandleTypeDef hspi)
{
	uint16_t zAxisData;

	uint8_t zDataL = LIS3MDL_ReadRegister(OUT_Z_L, hspi);
	uint8_t zDataH = LIS3MDL_ReadRegister(OUT_Z_H, hspi);

	zAxisData = (zDataH << 8) | zDataL;

	return TwosCompToDec(zAxisData);
}



int16_t TwosCompToDec(uint16_t value)
{
    // [0x0000; 0x7FFF] corresponds to [0; 32,767]
    // [0x8000; 0xFFFF] corresponds to [-32,768; -1]
    // int16_t has the range [-32,768; 32,767]

    uint16_t sign_mask = 0x8000;

    // if positive
    if ( (value & sign_mask) == 0 ) {
        return value;
    //  if negative
    } else {
        // invert all bits, add one, and make negative
        return -(~value + 1);
    }
}

void calibrate(MagnetometerRawData data, float *calib)
{
	// Converting raw data in gauss to teslas, substracting the bias offset
	float x_off = data.get_x() * 100000.0 / 1100.0 - BIAS_X;
	float y_off = data.get_y() * 100000.0 / 1100.0 - BIAS_Y;
	float z_off = data.get_z() * 100000.0 /  980.0 - BIAS_Z;

	// Soft Iron calibration
	calib[0] = SOFT_IRON_MATRIX_X[0] * x_off + SOFT_IRON_MATRIX_X[1] * y_off + SOFT_IRON_MATRIX_X[2] * z_off;
	calib[1] = SOFT_IRON_MATRIX_Y[0] * x_off + SOFT_IRON_MATRIX_Y[1] * y_off + SOFT_IRON_MATRIX_Y[2] * z_off;
	calib[2] = SOFT_IRON_MATRIX_Z[0] * x_off + SOFT_IRON_MATRIX_Z[1] * y_off + SOFT_IRON_MATRIX_Z[2] * z_off;
}

MagnetometerRawData::MagnetometerRawData() {
    set_x(0);
    set_y(0);
    set_z(0);
}

void MagnetometerRawData::get_axis_data(SPI_HandleTypeDef hspi) {
	set_x(LIS3MDL_GetXaxisData(hspi));
	set_y(LIS3MDL_GetYaxisData(hspi));
	set_z(LIS3MDL_GetZaxisData(hspi));
}

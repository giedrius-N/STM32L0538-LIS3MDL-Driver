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

	return twos_comp_to_dec(xAxisData);
}

int16_t LIS3MDL_GetYaxisData(SPI_HandleTypeDef hspi)
{
	uint16_t yAxisData;

	uint8_t yDataL = LIS3MDL_ReadRegister(OUT_Y_L, hspi);
	uint8_t yDataH = LIS3MDL_ReadRegister(OUT_Y_H, hspi);

	yAxisData = (yDataH << 8) | yDataL;

	return twos_comp_to_dec(yAxisData);
}

int16_t LIS3MDL_GetZaxisData(SPI_HandleTypeDef hspi)
{
	uint16_t zAxisData;

	uint8_t zDataL = LIS3MDL_ReadRegister(OUT_Z_L, hspi);
	uint8_t zDataH = LIS3MDL_ReadRegister(OUT_Z_H, hspi);

	zAxisData = (zDataH << 8) | zDataL;

	return twos_comp_to_dec(zAxisData);
}



int16_t twos_comp_to_dec(uint16_t value)
{
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

void handle_config_callback(ConfigHandler chandler, uint8_t result[]) {
	switch(chandler.get_power_mode()) {
		case LOW:
			result[0] = 'L';
			result[1] = 'P';
			break;
		case MEDIUM:
			result[0] = 'M';
			result[1] = 'P';
			break;
		case HIGH:
			result[0] = 'H';
			result[1] = 'P';
			break;
		case ULTRA_HIGH:
			result[0] = 'U';
			result[1] = 'H';
			break;
		default:
			result[0] = 'X';
			result[1] = 'X';
	}

	switch(chandler.get_temp()) {
		case TEMPON:
			result[2] = 'T';
			break;
		case TEMPOFF:
			result[2] = 'F';
			break;
		default:
			result[2] = 'X';
	}

	switch(chandler.get_blocking_mode()) {
		case BLOCKING:
			result[3] = 'B';
			break;
		case NONBLOCKING:
			result[3] = 'N';
			break;
		default:
			result[3] = 'X';
	}

	result[4] = '\r';
	result[5] = '\n';
}
int16_t MagnetometerRawData::get_x() {
	return x;
}
int16_t MagnetometerRawData::get_y() {
	return y;
}
int16_t MagnetometerRawData::get_z() {
	return z;
}

void MagnetometerRawData::set_x(int16_t x) {
	this->x = x;
}

void MagnetometerRawData::set_y(int16_t y) {
	this->y = y;
}

void MagnetometerRawData::set_z(int16_t z) {
	this->z = z;
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

ConfigHandler::ConfigHandler(SPI_HandleTypeDef hspi) {
	init_default_config(hspi);
}

uint8_t ConfigHandler::get_blocking_mode() {
	return blocking_mode;
}
uint8_t ConfigHandler::get_power_mode() {
	return power_mode;
}
uint8_t ConfigHandler::get_temp() {
	return temp;
}

void ConfigHandler::init_default_config(SPI_HandleTypeDef hspi) {
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

void ConfigHandler::process_config_req(uint8_t config_data[], SPI_HandleTypeDef hspi) {
	if (config_data[0] == 0 || config_data[3] == 0) {
		return;
	}

	this->power_mode = process_power_mode(config_data[0], config_data[1]);
	this->temp = process_temp(config_data[2]);
	this->blocking_mode = process_blocking_mode(config_data[3]);

	config(power_mode, temp, hspi);
}

uint8_t ConfigHandler::process_power_mode(uint8_t param1, uint8_t param2) {
	// If L and P - Low Power Mode
	if (param1 == 0x4C && param2 == 0x50) {
		return LOW;
	}
	// If M and P - Medium Performance Mode
	if (param1 == 0x4D && param2 == 0x50) {
		return MEDIUM;
	}
	// If H and P - High Performance Mode
	if (param1 == 0x48 && param2 == 0x50) {
		return HIGH;
	}
	// If U and H - Ultra High Performane Mode
	if (param1 == 0x55 && param2 == 0x48) {
		return ULTRA_HIGH;
	}

	//Default
	return ULTRA_HIGH;
}

uint8_t ConfigHandler::process_temp(uint8_t param) {
	// Enable temperature sensor
	if (param == 0x54) {
		return TEMPON;
	}
	// Disable temperature sensor
	if (param == 0x46) {
		return TEMPOFF;
	}

	// Default
	return TEMPON;
}

uint8_t ConfigHandler::process_blocking_mode(uint8_t param) {
	// If blocking mode
	if (param == 0x42) {
		return BLOCKING;
	}
	// If non-blocking mode
	if (param == 0x4E) {
		return NONBLOCKING;
	}

	// Default
	return NONBLOCKING;
}

void ConfigHandler::config(uint8_t power_mode, uint8_t temp, SPI_HandleTypeDef hspi) {
	if (power_mode == LOW) {
		if (temp) {
			//Set up LP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
			uint8_t ctrl_reg_1 = 0b10011100;
			LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
		}
		else {
			//Set up LP mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
			uint8_t ctrl_reg_1 = 0b00011100;
			LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
		}
		//Set Z axis mode to LP
		LIS3MDL_WriteRegister(CTRL_REG4, ZERO_VALUE, hspi);

		return;
	}

	if (power_mode == MEDIUM) {
		if (temp) {
			//Set up MP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
			uint8_t ctrl_reg_1 = 0b10111100;
			LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
		}
		else {
			//Set up MP mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
			uint8_t ctrl_reg_1 = 0b00111100;
			LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
		}
		//Set Z axis mode to MP
		uint8_t ctrl_reg_4 = 0b00000100;
		LIS3MDL_WriteRegister(CTRL_REG4, ctrl_reg_4, hspi);

		return;
	}

	if (power_mode == HIGH) {
			if (temp) {
				//Set up HP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
				uint8_t ctrl_reg_1 = 0b11011100;
				LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
			}
			else {
				//Set up HP mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
				uint8_t ctrl_reg_1 = 0b01011100;
				LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
			}
			//Set Z axis mode to HP
			uint8_t ctrl_reg_4 = 0b00001000;
			LIS3MDL_WriteRegister(CTRL_REG4, ctrl_reg_4, hspi);

			return;
	}

	if (power_mode == ULTRA_HIGH) {
			if (temp) {
				//Set up UH mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
				uint8_t ctrl_reg_1 = 0b11111100;
				LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
			}
			else {
				//Set up UH mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
				uint8_t ctrl_reg_1 = 0b01111100;
				LIS3MDL_WriteRegister(CTRL_REG1, ctrl_reg_1, hspi);
			}
			//Set Z axis mode to UH
			uint8_t ctrl_reg_4 = 0b00001100;
			LIS3MDL_WriteRegister(CTRL_REG4, ctrl_reg_4, hspi);

			return;
	}
}













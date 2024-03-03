/*
 * ConfigHandler.cpp
 *
 *  Created on: Feb 29, 2024
 *      Author: gnavi
 */

#include "configHandler.hpp"

ConfigHandler::ConfigHandler(SPI_HandleTypeDef *hspi) {
	init_default_config(*hspi);
}

BlockingMode ConfigHandler::get_blocking_mode() {
	return blocking_mode;
}
PowerMode ConfigHandler::get_power_mode() {
	return power_mode;
}
TempMode ConfigHandler::get_temp() {
	return temp;
}

void ConfigHandler::init_default_config(SPI_HandleTypeDef hspi) {
	//Set full scale to +- 12 Hz
	LIS3MDL_WriteRegister(CTRL_REG2, FULL_SCALE_12, hspi);

	//Set up UHP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
	LIS3MDL_WriteRegister(CTRL_REG1, XY_UHP_TEMP_ON, hspi);

	//Set up UHP mode on the Z axis
	LIS3MDL_WriteRegister(CTRL_REG4, Z_UHP, hspi);

	//Set continuous measurement mode
	LIS3MDL_WriteRegister(CTRL_REG3, ZERO_VALUE, hspi);

	//Turn on Block data update
	LIS3MDL_WriteRegister(CTRL_REG5, BLOCK_DATA_UPDATE, hspi);
}

void ConfigHandler::process_config_req(uint8_t *config_data, SPI_HandleTypeDef hspi) {
	this->power_mode = process_power_mode(config_data[0], config_data[1]);
	this->temp = process_temp(config_data[2]);
	this->blocking_mode = process_blocking_mode(config_data[3]);

	config(power_mode, temp, hspi);
}

PowerMode ConfigHandler::process_power_mode(uint8_t param1, uint8_t param2) {
	PowerMode mode;

	// If L and P - Low Power Mode
	if (param1 == 'L' && param2 == 'P') {
		mode = PowerMode::LOW;
	}
	// If M and P - Medium Performance Mode
	else if (param1 == 'M' && param2 == 'P') {
		mode = PowerMode::MEDIUM;
	}
	// If H and P - High Performance Mode
	else if (param1 == 'H' && param2 == 'P') {
		mode = PowerMode::HIGH;
	}
	// If U and H - Ultra High Performance Mode
	else if (param1 == 'U' && param2 == 'H') {
		mode = PowerMode::ULTRA_HIGH;
	}
	// Default to Ultra High Performance Mode
	else {
		mode = PowerMode::ULTRA_HIGH;
	}

	return mode;
}

TempMode ConfigHandler::process_temp(uint8_t param) {
	TempMode mode;

	// Enable temperature sensor
	if (param == 'T') {
		mode = TempMode::TEMPON;
	}
	// Disable temperature sensor
	else if (param == 'F') {
		mode = TempMode::TEMPOFF;
	}
	else {
		mode = TempMode::TEMPON;
	}

	return mode;
}

BlockingMode ConfigHandler::process_blocking_mode(uint8_t param) {
	BlockingMode mode;

	// If blocking mode
	if (param == 'B') {
		mode =  BlockingMode::BLOCKING;
	}
	// If non-blocking mode
	else if (param == 'N') {
		mode = BlockingMode::NONBLOCKING;
	}
	else {
		mode = BlockingMode::NONBLOCKING;
	}

	return mode;
}

void ConfigHandler::set_low_power(TempMode temp, SPI_HandleTypeDef hspi) {
	if (temp == TempMode::TEMPON) {
		//Set up LP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_LP_TEMP_ON, hspi);
	}
	else {
		//Set up LP mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_LP_TEMP_OFF, hspi);
	}
	//Set Z axis mode to LP
	LIS3MDL_WriteRegister(CTRL_REG4, ZERO_VALUE, hspi);
}

void ConfigHandler::set_medium_power(TempMode temp, SPI_HandleTypeDef hspi) {
	if (temp == TempMode::TEMPON) {
		//Set up MP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_MP_TEMP_ON, hspi);
	}
	else {
		//Set up MP mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_MP_TEMP_OFF, hspi);
	}
	//Set Z axis mode to MP
	LIS3MDL_WriteRegister(CTRL_REG4, Z_MP, hspi);
}

void ConfigHandler::set_high_power(TempMode temp, SPI_HandleTypeDef hspi) {
	if (temp == TempMode::TEMPON) {
		//Set up HP mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_HP_TEMP_ON, hspi);
	}
	else {
		//Set up HP mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_HP_TEMP_OFF, hspi);
	}
	//Set Z axis mode to HP
	LIS3MDL_WriteRegister(CTRL_REG4, Z_HP, hspi);
}

void ConfigHandler::set_ultra_high_power(TempMode temp, SPI_HandleTypeDef hspi) {
	if (temp == TempMode::TEMPON) {
		//Set up UH mode on the X/Y axis, ODR at 80 Hz, activates temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_UHP_TEMP_ON, hspi);
	}
	else {
		//Set up UH mode on the X/Y axis, ODR at 80 Hz, disable temp sensor
		LIS3MDL_WriteRegister(CTRL_REG1, XY_UHP_TEMP_OFF, hspi);
	}
	//Set Z axis mode to UH
	LIS3MDL_WriteRegister(CTRL_REG4, Z_UHP, hspi);
}

void ConfigHandler::config(PowerMode power_mode, TempMode temp, SPI_HandleTypeDef hspi) {
	switch(power_mode) {
		case PowerMode::LOW:
			set_low_power(temp, hspi);
			break;
		case PowerMode::MEDIUM:
			set_medium_power(temp, hspi);
			break;
		case PowerMode::HIGH:
			set_medium_power(temp, hspi);
			break;
		case PowerMode::ULTRA_HIGH:
			set_ultra_high_power(temp, hspi);
			break;
		default:
			return;
	}
}

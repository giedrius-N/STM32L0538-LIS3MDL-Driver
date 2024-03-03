/*
 * MagnetometerRawData.cpp
 *
 *  Created on: Feb 29, 2024
 *      Author: gnavi
 */

#include "magnetometerRawData.hpp"

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
    set_x(ZERO_VALUE);
    set_y(ZERO_VALUE);
    set_z(ZERO_VALUE);
}

void MagnetometerRawData::get_axis_data(SPI_HandleTypeDef hspi) {
	set_x(LIS3MDL_GetXaxisData(hspi));
	set_y(LIS3MDL_GetYaxisData(hspi));
	set_z(LIS3MDL_GetZaxisData(hspi));
}

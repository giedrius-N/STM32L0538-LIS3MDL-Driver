/*
 * MagnetometerRawData.cpp
 *
 *  Created on: Feb 29, 2024
 *      Author: gnavi
 */

#ifndef INC_MAGNETOMETERRAWDATA_CPP_
#define INC_MAGNETOMETERRAWDATA_CPP_

#include "lis3mdl.hpp"
#include <stdio.h>
#include <cstdint>
#include "main.h"

class MagnetometerRawData {
	private:
		int16_t x;
		int16_t y;
		int16_t z;

	public:
		MagnetometerRawData();

		void set_x(int16_t x);
		int16_t get_x(void);

		void set_y(int16_t y);
		int16_t get_y(void);

		void set_z(int16_t z);
		int16_t get_z(void);

		void get_axis_data(SPI_HandleTypeDef hspi);
};


#endif /* INC_MAGNETOMETERRAWDATA_CPP_ */

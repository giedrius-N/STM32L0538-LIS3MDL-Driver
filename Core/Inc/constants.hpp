/*
 * Constants.hpp
 *
 *  Created on: Feb 29, 2024
 *      Author: gnavi
 */

#ifndef INC_CONSTANTS_HPP_
#define INC_CONSTANTS_HPP_

#define ZERO_VALUE 0x00
#define FULL_CIRCLE_DEGREES 360
#define HALF_CIRCLE_DEGREES 180
#define ANGLE_225 225
#define ANGLE_315 315
#define ANGLE_45 45
#define ANGLE_135 135
#define ONE_SECOND 1000

#define MAX_HEADING_SIZE 8
#define CONFIG_CALLBACK_SIZE 6

#define FULL_SCALE_12 0x40

#define XY_LP_TEMP_ON 0x9C
#define XY_LP_TEMP_OFF 0x1C
#define XY_MP_TEMP_ON 0xBC
#define XY_MP_TEMP_OFF 0x3C
#define XY_HP_TEMP_ON 0xDC
#define XY_HP_TEMP_OFF 0x5C
#define XY_UHP_TEMP_ON 0xFC
#define XY_UHP_TEMP_OFF 0x7C

#define Z_MP 0x04
#define Z_HP 0x08
#define Z_UHP 0x0C

#define BLOCK_DATA_UPDATE 0xC0

#define SCALE_FACTOR_X  (100000.0 / 1100.0)
#define SCALE_FACTOR_Y  (100000.0 / 1100.0)
#define SCALE_FACTOR_Z  (100000.0 /  980.0)

//Bias from magneto1.2 application
#define BIAS_X 67020.821058
#define BIAS_Y -93341.600766
#define BIAS_Z -7830.178850
#define MAG_DECLINATION 5  //Magnetic declination in Czech Republic

//Soft iron callibration data from magneto1.2
const float SOFT_IRON_MATRIX_X[] = {0.359843, 0.025408, -0.043194};
const float SOFT_IRON_MATRIX_Y[] = {0.025408, 0.422331, -0.008441};
const float SOFT_IRON_MATRIX_Z[] = {-0.043194, -0.008441, 0.370303};


enum class PowerMode {
	LOW,
	MEDIUM,
	HIGH,
	ULTRA_HIGH
};

enum class TempMode {
	TEMPOFF,
	TEMPON
};

enum class BlockingMode {
	BLOCKING,
	NONBLOCKING
};

#endif /* INC_CONSTANTS_HPP_ */

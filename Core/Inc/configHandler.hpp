/*
 * ConfigHandler.hpp
 *
 *  Created on: Feb 29, 2024
 *      Author: gnavi
 */

#ifndef INC_CONFIGHANDLER_HPP_
#define INC_CONFIGHANDLER_HPP_

#include "lis3mdl.hpp"
#include "constants.hpp"

class ConfigHandler {
	private:
		BlockingMode blocking_mode;
		PowerMode power_mode;
		TempMode temp;
	public:
		ConfigHandler(SPI_HandleTypeDef *hspi);

		BlockingMode get_blocking_mode();
		PowerMode get_power_mode();
		TempMode get_temp();

		void init_default_config(SPI_HandleTypeDef hspi);
		void process_config_req(uint8_t *configData, SPI_HandleTypeDef hspi);

		PowerMode process_power_mode(uint8_t param1, uint8_t param2);
		TempMode process_temp(uint8_t param);
		BlockingMode process_blocking_mode(uint8_t param);

		void set_low_power(TempMode temp, SPI_HandleTypeDef hspi);
		void set_medium_power(TempMode temp, SPI_HandleTypeDef hspi);
		void set_high_power(TempMode temp, SPI_HandleTypeDef hspi);
		void set_ultra_high_power(TempMode temp, SPI_HandleTypeDef hspi);
		void config(PowerMode power_mode, TempMode temp, SPI_HandleTypeDef hspi);
};


#endif /* INC_CONFIGHANDLER_HPP_ */

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



uint8_t LIS3MDL_ReadRegister(uint8_t regAddr, SPI_HandleTypeDef hspi);
void LIS3MDL_WriteRegister(uint8_t regAddr, uint8_t data, SPI_HandleTypeDef hspi);


#endif /* INC_LIS3MDL_HPP_ */

/***************************************************************************
 * Project           		 : shakti devt board
 * Name of the file	         : lm75_softi2c.c
 * Brief Description of file     : Helps to find out temperature of a surrounding with the help of gpio pins.
 * Name of Author                : Soutrick Roy Chowdhury.
 * Email ID                      : soutrick97@gmail.com

 Copyright (C) 2019  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

*****************************************************************************/
/**
@file lm75_softi2c.c
@brief Implements gpio functionality.
@detail Helps to find out temperature of a surrounding with the help of gpio pins.
*/

#include "gpio_i2c.h"

#define LM75_SLAVE_ADDRESS 0x90
#define DELAY_COUNT 200

/** @fn unsigned int Lm75ReadTemperature(unsigned long delay)
 * @brief Complete function to read value from LM75 sensors 
 * @param unsigned long delay
 * @return unsigned int
 */
unsigned int Lm75ReadTemperature(unsigned long delay)
{
	unsigned char readBuf = 0;
	unsigned short readValue = 0;
	I2cSendSlaveAddress(LM75_SLAVE_ADDRESS, I2C_READ, delay);
	readBuf = I2cReadDataAck(delay);
	readValue = (readBuf << 1);
	readBuf = I2cReadDataNack(delay);
	I2cStop(delay);  
	readValue = (readBuf >> 7) | readValue;
	return readValue;
}

/** @fn void main()
 * @brief Calling Lm75ReadTemperature to find the temperature
 * @details Printing the Current temperature
 */
void main() 
{
	unsigned short tempRead = 0;
	unsigned short temp = 0;

	printf("\n\ttemp sensor initiating!");

	I2cInit();

	tempRead = Lm75ReadTemperature(DELAY_COUNT);
	if(tempRead & 0x100) {
		tempRead = ( (~tempRead) + 1 ) >> 1;
		printf("\n\t Current Temperature:=-%u°C",tempRead);
	}		
	else {
		tempRead = tempRead >> 1;
		printf("\n\t Current Temperature:=+%u°C",tempRead);
	 }
	delay_loop(10000,20000);
}

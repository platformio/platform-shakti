/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  keypad.h
* Created date			        :  26.02.2019
* Brief Description of file             :  Declaration for keypad library.

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

***************************************************************************/
/* ------------------------------------------------------------ */
/*	     Include File Definitions			            		*/
/* ------------------------------------------------------------ */
#if !defined(KYPD_H)
#define KYPD_H

#define KYPD_ROWNUM 4
#define KYPD_COLNUM 4

#define GPIO_START 0X00011100
#define GPIO_DIRECTION_CNTRL_REG (GPIO_START + 0X00)
#define GPIO_DATA_REG  (GPIO_START + 0X04)
#define GPIO_DRV_CNTRL_REG0 (GPIO_START + 0X08)
#define GPIO_DRV_CNTRL_REG1 (GPIO_START + 0X0C)
#define GPIO_DRV_CNTRL_REG2 (GPIO_START + 0X10)
#define GPIO_PULL_DOWN_CNTRL_REG (GPIO_START + 0X14)
#define GPIO_MODE_SEL_REG (GPIO_START + 0X18)
#define GPIO_PROG_SLEW_CNTRL_REG (GPIO_START + 0X1C)
#define GPIO_PULL_UP_CNTRL_REG (GPIO_START + 0X20)
#define CORE_PWR_DOWN_STS_REG0 (GPIO_START + 0X24)
#define CORE_PWR_DOWN_STS_REG1 (GPIO_START + 0X28)
#define OUTPINS_OFFSET 24
#define INPINS_OFFSET 16
#define OUTPINS (0xF << OUTPINS_OFFSET)
#define INPINS  (0xF << INPINS_OFFSET)
/* ------------------------------------------------------------ */
/*				Procedure Declarations						*/
/* ------------------------------------------------------------ */



    void setPins(unsigned int* row, unsigned int* col);
    void setKeyMap(int table[KYPD_COLNUM][KYPD_ROWNUM]);  
    int getKey();
    int getColRow(void);
    unsigned int rowPins[KYPD_ROWNUM];
    unsigned int colPins[KYPD_COLNUM];
    int keyMap[KYPD_COLNUM][KYPD_ROWNUM];
#endif

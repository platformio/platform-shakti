/************************************************************************
 * Project           			:  shakti devt board
 * Name of the file	     		:  keypad.c
 * Created date			        :  26.02.2019
 * Brief Description of file     : This is an example on how to get the value of a key  pressed on a Pmod keypad.

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
/*
   To Do: identify a key press to a letter. some more testing.
*/

/***********************************************************
*		Include File Definitions			
************************************************************/

#include "gpio.h"
#include "platform.h"
#include "keypad.h"//Includes the definitions of keypad function//

/* global variable definition*/

// initialize key and colrow to -1 indicate no key is pressed
int key = -1;
int colRow = -1;

unsigned int rowPins[KYPD_ROWNUM];
unsigned int colPins[KYPD_COLNUM];
int keyMap[KYPD_COLNUM][KYPD_ROWNUM];

unsigned int col[4]={3, 2, 1, 0};
unsigned int row[4]={7, 6, 5, 4};


int  keyTable[4][4]={{1, 4, 7, 15},    
	{2, 5, 8, 0},    
	{3, 6, 9, 14},    
	{10, 11, 12, 13}};

/************************************************************
*	Description:
*	This function sets the pins for the row and column
************************************************************/
void  setPins(unsigned int*  row, unsigned int* col)
{    
	for(int i = 0 ; i < KYPD_COLNUM ; i++)       
	{
		colPins[i] = col[i];// set col
	}
	for(int j = 0 ; j < KYPD_ROWNUM ; j++) // set row       
	{
		rowPins[j] = row[j];
	}
}

/************************************************************
*	Description:
*	This function maps table in to keymap
************************************************************/

void setKeyMap(int table[KYPD_COLNUM][KYPD_ROWNUM])
{    

	for(int i = 0 ; i < KYPD_COLNUM ; i++)       
	{
		for(int j = 0 ; j < KYPD_ROWNUM ; j++)        
		{
			keyMap[i][j] = table[i][j];
		}
	}    
}

/************************************************************
*	Description:
*	This function returns the corresponding value in the keymap
************************************************************/

int getKey()
{    

	write_word(GPIO_DATA_REG, (0xF << OUTPINS_OFFSET) );   

	for(int i = 0 ; i < KYPD_COLNUM ; i++)       
	{ 
		write_word(GPIO_DRV_CNTRL_REG2, ~(0x1 << (OUTPINS_OFFSET + i) )   );   

		for(int j = 0 ; j < KYPD_ROWNUM ; j++)       
		{         
			if( ( read_word(rowPins[j]) & (INPINS << INPINS_OFFSET )  ) == 0)
			{
				return keyMap[i][j];   
			}
		}
		write_word(GPIO_DATA_REG, (0xF << OUTPINS_OFFSET) );   
	}

	return 0;
}

/************************************************************
 *	Description:
 *	This function returns the column-row
 ***********************************************************/

int getColRow(void)
{    
	int colRow = 0;
	unsigned long readValue = 0;

	write_word(GPIO_DATA_REG, (0xF << OUTPINS_OFFSET) );   


	for(int i = 0 ; i < KYPD_COLNUM ; i++)       
	{ 
		write_word(GPIO_DATA_REG, ~(0x1 << (OUTPINS_OFFSET + i) )   );   

		for(int j = 0 ; j < KYPD_ROWNUM ; j++)       
		{ 
			readValue =  read_word(GPIO_DATA_REG) ;
			printf("\n The read value is %lx; rowPins: %d", (readValue & 0x0F0F0000), rowPins[j]);

			if( ( readValue & (1 << rowPins[j] ) == 0) )
			{
				printf("\n The read value is %lx; rowPins: %d", readValue, rowPins[j]);

				return (i << 16 | j);
			}
		}    

		write_word(GPIO_DATA_REG, (0xF << OUTPINS_OFFSET) );   
	}

	return 0;
}

/*********************************************************************
* Brief Description     :This maps the keypad from pins. 
* Parameters            :rows,columns.
********************************************************************/

void gpio_init()
{
	setPins(row, col);

	setKeyMap(keyTable);

#ifndef ARTIX7_35T 
	//Configure the GPIO pins
	write_word(GPIO_DRV_CNTRL_REG0, OUTPINS);
	write_word(GPIO_DRV_CNTRL_REG1, OUTPINS);
	write_word(GPIO_DRV_CNTRL_REG2, 0x00);

	//Makes the output as pushpull (0 ---> open drain, 1 ---> pushpull)
	write_word(GPIO_MODE_SEL_REG, OUTPINS);

	//Makes the slewrate as slow (0 ---> slow, 1 ---> fast)
	write_word(GPIO_PROG_SLEW_CNTRL_REG, 0x00);
#endif

	//Configure direction of GPIO pins
	write_word(GPIO_DIRECTION_CNTRL_REG, OUTPINS);  
}

int main() 
{
	gpio_init(); //Intialization of gpio //

	printf("\nGPIO Init Done");

	while(1)
	{
		colRow = getColRow();

		if( colRow != -1 )
		{
			key = getKey();
			printf("\ncol: %d; row: %d is value: %d", colRow>>16, (0xFFFF & colRow), key ); 
		}

		DelayLoop(100,100);
		DelayLoop(5000, 5000); 
	}

	return 0;
}

/******************************************************************************************
* Copyright 2017 Ideetron B.V.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************************/
/****************************************************************************************
* File:     th06.h
* Author:   Adri Verhoef
* Company:	Ideetron B.V.
* Website:  http://www.ideetron.nl/LoRa
* E-mail:   info@ideetron.nl
* Created on:         03-10-2017
* Supported Hardware: ID150119-02 Nexus demo board
****************************************************************************************/

#ifndef TH06_H
#define TH06_H

	/*********************************************************************************************
											INCLUDES
	*********************************************************************************************/
	#include <stdint.h>
	#include <stdbool.h>


	/*********************************************************************************************
											DEFINITIONS
	*********************************************************************************************/
	
	#define THO6_I2C_ADDRESS 0x40
	#define TH06_RSVD_MASK   0b00111010 // bit mask for the temperature user register 1
	
	/*********************************************************************************************
											ENUMERATIONS
	*********************************************************************************************/
	
	/* TH06 Registers */
	typedef enum
	{
		TH06_MEASURE_HUMIDITY_HOLD_MASTER           = 0xE5,
		TH06_MEASURE_HUMIDITY_NO_HOLD_MASTER        = 0xF5,
		TH06_MEASURE_TEMPERATURE_HOLD_MASTER        = 0xE3,
		TH06_MEASURE_TEMPERATURE_NO_HOLD_MASTER     = 0xF3,
		TH06_MEASURE_TEMPERATURE_PREVIOUS           = 0xE0,
		TH06_RESET                                  = 0xFE,
		TH06_WRITE_USER_REG1                        = 0xE6,
		TH06_READ_USER_REG1                         = 0xE7,
		TH06_FIRMWARE_REV                           = 0x84
	}TH06_I2C_COMMANDS;

	typedef enum
	{
		RES_MASK        = 0b10000001,	// mask for bits 7 and 0
		RES_RH12_Temp14 = 0b00000000,   //  22.8ms MAX conversion time
		RES_RH8_Temp12  = 0b00000001,   //  6.9ms
		RES_RH10_Temp13 = 0b10000000,   //  23.0ms
		RES_RH11_Temp11 = 0b10000001    //  9.4ms
	}TEMP_RESOLUTION;


	/* Heater enumeration to enable or disable the heater on initialization. */
	typedef enum
	{
		HEATER_ON  = 0x04,
		HEATER_OFF = 0x00
	}HEATERe;
	
	/******************************************************************************************
											STRUCTURE
	******************************************************************************************/
	
	/* Structure for the temperature and humidity results. */
	typedef struct
	{
		double		Temperature;
		double		Humidity;
	}TH06_DATA;

	/******************************************************************************************
										FUNCTION PROTOTYPES
	******************************************************************************************/
	
	void    TH06_init						(HEATERe Mode, TEMP_RESOLUTION Resolution);
	bool    TH06_sample						(TH06_DATA * measured);
	void	TH06_print_temp_and_humidity	(TH06_DATA * print);

#endif
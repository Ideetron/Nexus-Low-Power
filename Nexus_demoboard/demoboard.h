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
* File:     mcp7940.h
* Author:   Adri Verhoef
* Company:	Ideetron B.V.
* Website:  http://www.ideetron.nl/LoRa
* E-mail:   info@ideetron.nl
* Created on:         22-09-2017
* Supported Hardware: ID150119-02 Nexus board with RFM95
****************************************************************************************/

#ifndef DEMOBOARD_H_
#define DEMOBOARD_H_

	/*********************************************************************************************
										TYPE DEFINITION
	*********************************************************************************************/


	/******************************************************************************************
											DEFINES
	******************************************************************************************/

	#define MOVEMENT	PIN_A0
	#define BUTTON1		PIN_A1
	#define BUTTON2		PIN_A2
	#define LDR			PIN_A7
	#define POT_METER	PIN_A6
	
	/******************************************************************************************
									FUNCTION PROTOTYPES
	******************************************************************************************/
	void		movement_sensor_init			(void);
	double		read_supply_voltage				(void);
	void		demoboard_enable_button_wakeup	(void);


#endif /*  */
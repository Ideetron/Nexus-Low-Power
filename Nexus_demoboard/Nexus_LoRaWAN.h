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
* File:     Nexus_LoRaWAN.h
* Author:   Gerben den Hartog
* Company:	Ideetron B.V.
* Website:  http://www.ideetron.nl/LoRa
* E-mail:   info@ideetron.nl
* Created on:         06-01-2017
* Supported Hardware: ID150119-02 Nexus board with RFM95
****************************************************************************************/

#ifndef LORAWAN_H
#define LORAWAN_H

	/*********************************************************************************************
										TYPE DEFINITION
	*********************************************************************************************/


	/******************************************************************************************
											DEFINES
	******************************************************************************************/

	#define DS2401		2	// One wire pin for the DS2401
	#define RTC_MFP		3
	#define RFM_DIO0    4
	#define RFM_DIO1    5
	#define RFM_DIO5    6
	#define RFM_DIO2    7
	#define RFM_NSS		10
	#define EE_CS		8	// Pin for the additional EEPROM memory
	#define LED			9


	/******************************************************************************************
											STRUCTURES
	******************************************************************************************/
	typedef struct
	{
		uint8_t		DS2401_bytes[8];				//	DS2401 data bytes.
		uint16_t	LoRaWAN_message_interval = 1;	//	Variable to set the number of Timer2 timer ticks between LoRaWAN messages. 	
		uint16_t	Movement;						//	Movement sensor interrupt count
		uint16_t	LDR_value;						//	ADC value of the LDR on the Nexus Demoboard 
		uint16_t	POT_value;						//	ADC value of the Potentiometer on the Nexus Demoboard
		double		SupplyVoltage;					//	Supply voltage as a fractional value
	}sAPP;
	
	/******************************************************************************************
									FUNCTION PROTOTYPES
	******************************************************************************************/
	
	void	go_to_sleep			(void);
	void	printStringAndHex	(const char *String, uint8_t *data, uint8_t n);
	void	handle_reply		(void);
	void	send_button_press	(uint8_t Button);
	
#endif


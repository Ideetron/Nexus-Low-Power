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
* File:     Nexus_keyfob.ino
* Author:   Adri Verhoef & Gerben den Hartog for the original Nexus Sketch
* Company:	Ideetron B.V.
* Website:  http://www.ideetron.nl/LoRa
* E-mail:   info@ideetron.nl
* Created on:         13-09-2017
* Supported Hardware: ID150119-02 Nexus board with RFM95
* Description
*	This firmware demonstrates how to make a key fob with a Nexus Development board and 
*	the Nexus demoboard or two buttons
****************************************************************************************/

/*
*****************************************************************************************
* INCLUDE FILES
*****************************************************************************************
*/
#include <avr/power.h>
#include <avr/sleep.h>
#include <stdbool.h>


#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>


#include "AES-128.h"
#include "Encrypt.h"
#include "Nexus_keyfob.h"
#include "RFM95.h"
#include "LoRaMAC.h"
#include "timers.h"
#include "DS2401.h"
#include "lorawan_def.h"
#include "I2C.h"
#include "mcp7940.h"
#include "th06.h"
#include "spi_flash.h"
#include "nexus_demoboard.h"
#include "Cayenne_LPP.h"


/******************************************************************************************
									GLOBAL VARIABLES
******************************************************************************************/

	// Interrupt variables.
	volatile bool	BUTTONS_ACTIVE;

	sAPP		app;		//	Application variables
	sLoRaWAN	lora;		//	See the Nexus_Lorawan.h file for the settings of this structure.

	// Initialize the LoRaWAN stack.
	LORAMAC lorawan (&lora);
	
	// Initialize the Low Power Protocol functions
	CayenneLPP LPP(&(lora.TX));


/******************************************************************************************
									INTERRUPTS
******************************************************************************************/

/*
	@Brief	Interrupt vector for the movement sensor on the Nexus Demoboard, which will generate an interrupt on the Pin Change interrupt pin.
*/
ISR(PCINT1_vect)
{
	// Check if the buttons are pressed, but handle their functionality in the main loop.
	if((digitalRead(BUTTON1) == LOW) || (digitalRead(BUTTON2) == LOW))
	{
		BUTTONS_ACTIVE = true;
	}
}


/*
  @Brief  Interrupt vector for Timer1 which is used to time the Join and Receive windows for timeslot 1 and timeslot 2
*/
ISR(TIMER1_COMPA_vect)
{
	// Increment the timeslot counter variable for timing the JOIN and receive delays. 
	lora.timeslot++;
}




/******************************************************************************************
									MAIN
******************************************************************************************/
void setup(void)
{
	
	//Initialize the UART on 9600 baud 8N1
	Serial.begin(9600);

	////Initialize the SPI port
	SPI.begin();
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
	
	//Initialize I/O pins for the RFM95, DS2401 and MCP
	pinMode(A6,				INPUT);
	pinMode(A7,				INPUT);
	pinMode(DS2401, 		OUTPUT);
	pinMode(MOVEMENT,		INPUT);
	pinMode(RTC_MFP, 		INPUT);
	pinMode(RFM_DIO0, 		INPUT);
	pinMode(RFM_DIO1, 		INPUT);
	pinMode(RFM_DIO5, 		INPUT);
	pinMode(RFM_DIO2, 		INPUT);
	pinMode(RFM_NSS,		OUTPUT);
	pinMode(SPI_FLASH_CS,	OUTPUT);
	pinMode(LED,  			OUTPUT);
	
	digitalWrite(DS2401,		LOW);
	digitalWrite(SPI_FLASH_CS,	HIGH);
		
	// Power on delay for the RFM module
	delay(3000);
		
	// Initialize the I2C bus
	I2C_init();
	
	// Initialize the RFM95, some make sure that the SPI has been enabled.
	lorawan.init();
		
	// Flash power down to reduce power consumption.
	flash_power_down();
	
	sei();// Enable Global Interrupts	
}



void loop()
{	
	/*
		Get unique ID from the DS2401 and check the CRC. If the CRC matches, print the contents to the serial output. The unique ID is used 
		as Device EUI to generate a unique ID for each Nexus Board. The following code can be remove and the Device EUI can be set hard coded
		in the lora structure in lorawan_def.h if a different Dev EUI will be used.
	*/
	while(DS_Read(&(lora.OTAA.DevEUI[0])) == false)
	{}
	printStringAndHex("DS2401 DEV EUI: ", &(lora.OTAA.DevEUI[0]), 8);
	
	// Connect to the back-end with OTAA when OTAA is selected as the activation method.
	lorawan.OTAA_connect();
	
	// Disable the RTC for lower power consumption
	mcp7940_disable();
			
	// Enable the buttons on the demo board to wake-the atmega328p from POWER_DOWN 
	demoboard_enable_button_wakeup();
	BUTTONS_ACTIVE = false;

	// Super loop
	while(1)
	{	
		if(BUTTONS_ACTIVE == true)
		{
			// Wait a few ms to clear the bounce from the buttons and then check the pin states
			delay(500);
			
			// Check whether both buttons are pressed.
			if((digitalRead(BUTTON1) == LOW) && (digitalRead(BUTTON2) == LOW))
			{
				send_button_press(3);
			}
			// Check if only button1 is pressed.
			else if (digitalRead(BUTTON1) == LOW)
			{
				send_button_press(1);
			}
			// Check if only button2 is pressed.
			else if (digitalRead(BUTTON2) == LOW)
			{
				send_button_press(2);
			}
			
			// Clear the flag
			BUTTONS_ACTIVE = false;
		}
		else
		{	
			// Go to POWER_DOWN_MODE to reduce power consumption when the Arduino has nothing to do.		
			go_to_sleep();
		}	
	}//While(1)
}


/*
	@brief
		Function which disables all peripherals and enters SLEEP_MODE_PWR_DOWN mode to reduce power consumption to 0.1uA. When an interrupts wakes the MCU
		from sleep, the peripherals and interrupts are enabled again.
*/
void go_to_sleep (void)
{
	// Complete serial transfer before going to sleep
	Serial.flush();
				
	// Power off the peripherals
	ADCSRA &= ~(1 << ADEN);
	power_usart0_disable();
	power_timer0_disable();
	power_timer1_disable();
	power_timer2_disable();
	power_adc_disable();
	power_spi_disable();
	power_twi_disable();
				
	// Go to sleep mode and wake-up from the RTC's alarm or an Pin change Interrupt from the movement sensor.
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	cli();
	sleep_enable();
	sleep_bod_disable();
	sei();
	sleep_cpu();
	sleep_disable();
	sei();
				
	// Power on the peripherals
	power_usart0_enable();
	power_timer0_enable();
	power_timer1_enable();
	power_timer2_enable();
	power_adc_enable();
	power_spi_enable();
	power_twi_enable();
	ADCSRA |= (1 << ADEN);
}



/*
	@brief
		Function to print a string of text and an array of data in hexadecimal format.
	@parameters
		String	String of characters, must be zero terminated.
		data	Pointer to the array that must be printed in hexadecimal format
		n		Number of bytes that must be printed out in hexadecimal
*/
void printStringAndHex(const char *String, uint8_t *data, uint8_t n)
{
	uint8_t i;
	
	Serial.print(String);
	Serial.flush();
	Serial.print(n, DEC);
	Serial.print(" bytes; ");
	
	// Print the data as a hexadecimal string
	for( i = 0 ; i < n ; i++)
	{
		// Print single nibbles, since the Hexadecimal format printed by the Serial.Print function does not print leading zeroes.
		Serial.print((unsigned char) ((data[i] & 0xF0) >> 4), HEX); // Print MSB first
		Serial.print((unsigned char) ((data[i] & 0x0F) >> 0), HEX); // Print LSB second
		Serial.print(' ');
		Serial.flush();
	}
	Serial.println();
}


/*
	@brief 
*/
void send_button_press(uint8_t Button)
{
	// Transmit the button value with a digital output.
	LPP.clearBuffer();
	LPP.addDigitalOutput(0x03, Button);
	
	// Transmit as unconfirmed up
	lora.TX.Confirmation = UNCONFIRMED;	
	lorawan.LORA_Send_Data();
	
	// Print a message to the serial interface to give some feedback to the user.
	Serial.println();
	Serial.print("Button message value: ");
	Serial.println(Button, DEC);
}

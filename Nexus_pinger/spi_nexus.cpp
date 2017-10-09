// 
// 
// 

#include <stdint.h>
#include <SPI.h>
#include "LoRaMAC.h"
#include "RFM95.h"
#include "spi_nexus.h"
#include "Nexus_LoRaWAN.h"
#include "Arduino.h"
#include "timers.h"
#include "lorawan_def.h"
#include "spi_nexus.h"




/******************************************************************************************
* Description : Function that reads a register from the RFM and returns the value
*
* Arguments   : RFM_Address Address of register to be read
*
* Returns   : Value of the register
******************************************************************************************/

uint8_t SPI_Read(uint8_t CS_pin, uint8_t register_Address)
{
	uint8_t RFM_Data;

	// Set NSS pin low to start SPI communication
	digitalWrite(CS_pin, LOW);

	// Send Address
	SPI.transfer(register_Address);
	//Send 0x00 to be able to receive the answer from the RFM
	RFM_Data = SPI.transfer(0x00);

	// Set NSS high to end communication
	digitalWrite(CS_pin, HIGH);

	// Return received data
	return RFM_Data;
}

/******************************************************************************************
* Description:
* Function that writes a register of the RFM wit the given value
*
* Arguments:
* RFM_Address Address of the RFM register to be written
* RFM_Data    Data to be written to the register
******************************************************************************************/
void SPI_Write(uint8_t CS_pin, uint8_t register_Address, uint8_t Data)
{
	//Set NSS pin Low to start communication
	digitalWrite(CS_pin, LOW);

	//Send Address with MSB 1 to make it a writ command
	SPI.transfer(register_Address | 0x80);
	
	//Send Data
	SPI.transfer(Data);

	//Set NSS pin High to end communication
	digitalWrite(CS_pin, HIGH);
}


/**********************************************************************************************************************************************
* Description:
* Function that writes an array of data to the RFM register
*
* Arguments:
* RFM_Address	Address of the RFM register to be written too.
* RFM_Data		pointer to the Data array to be written to the RFM, starting on to the given register address
* lenght		The number of bytes needed to transmit
**********************************************************************************************************************************************/
void SPI_Write_Array(uint8_t CS_pin, uint8_t register_Address, uint8_t *RFM_Data, uint8_t lenght)
{
	//Set NSS pin Low to start communication
	digitalWrite(CS_pin, LOW);

	//Send Address with MSB 1 to make it a Write command
	SPI.transfer(register_Address | 0x80);
	
	//Send the data array
	SPI.transfer(RFM_Data, lenght);

	//Set NSS pin High to end communication
	digitalWrite(CS_pin, HIGH);
}

/**********************************************************************************************************************************************
* Description:
* Function that writes an array of data to the RFM register
*
* Arguments:
* RFM_Address	Address of the RFM register to be written too.
* RFM_Data		pointer to the Data array to be written to the RFM, starting on to the given register address
* lenght		The number of bytes needed to transmit
**********************************************************************************************************************************************/
void SPI_Read_Array(uint8_t CS_pin, uint8_t register_Address, uint8_t *Data, uint8_t lenght)
{
	//Set Chip select pin low to start SPI communication
	digitalWrite(CS_pin, LOW);

	//Send the register Address and then read the contents of the receive buffer in the RFM
	SPI.transfer(register_Address);
	SPI.transfer(Data, lenght);

	//Set NSS high to end communication
	digitalWrite(CS_pin, HIGH);
}



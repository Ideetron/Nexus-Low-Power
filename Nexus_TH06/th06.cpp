#include "I2C.h"
#include "th06.h"
#include "HardwareSerial.h"

/*
 **********************************************************************************************************************************************************************************
 * /title   void TH06_init(void)
 * /brief   
 * /detail  
**********************************************************************************************************************************************************************************
 */
void TH06_init(HEATERe Mode, TEMP_RESOLUTION Resolution)
{
    /* Setup the TH06 with the highest resolution and the heater disabled. */
    uint8_t read;
    
    /* Read the user register first so we can keep the reserved bits in the register on their original values. */
	read = I2C_read_register(THO6_I2C_ADDRESS, TH06_READ_USER_REG1);
	  
    // Clear all bits except for the reserved bits, keep their values.
    read &= TH06_RSVD_MASK;
    
    // Set the resolution bits
    read |= Resolution;
    
    // Set the heater bit to 1 if enabled.
    if(Mode == HEATER_ON)
    {
        read |= HEATER_ON;
    }
    
    /* Configure the TH06 with the specified resolution and heater setting. */
	I2C_write_register(THO6_I2C_ADDRESS, TH06_WRITE_USER_REG1, read);
}


/***********************************************************************************************************************************************************************************
 * /title   void TH06_sample(void)
 * /brief   Starts a temperature conversation and let the TH06 hold the I2C bus until the conversion is complete. Then return the Temperature and humidity. 
 * /param	measured	 Pointer to the structure where the conversion results needs to be stored.		
***********************************************************************************************************************************************************************************/
bool TH06_sample(TH06_DATA * measured)
{
    uint8_t tempResult[2], rhResult[2];
    uint16_t reg;
	
	if(measured == NULL)
	{
		return;
	}		
    
    /* Send a command to measure the humidity, which will measure the temperature as well and use clock stretching until the TH06 is done*/
	I2C_read_array(THO6_I2C_ADDRESS, TH06_MEASURE_HUMIDITY_HOLD_MASTER, &(rhResult[0]), 2);
     
    /* Read the conversion result and Calculate the Relative Humidity */
    reg = (((uint16_t) rhResult[0]) << 8) | (uint16_t) rhResult[1];
    if(reg != 0)
    {
        measured->Humidity =  ((125.0 * ((double) reg)) / 65536.0) - 6.0;
    }
    else
    {
        measured->Humidity = 0.0;
    }
    
    
    /* Read the temperature result */
	I2C_read_array(THO6_I2C_ADDRESS, TH06_MEASURE_TEMPERATURE_PREVIOUS, &(tempResult[0]), 2);
    
    /* Calculate the Temperature */
    reg = (((uint16_t) tempResult[0]) << 8) | (uint16_t) tempResult[1];
    if(reg != 0)
    {
        measured->Temperature = ((175.72 * ((double) reg)) / 65536.0) - 46.85;
    }
    else
    {
        measured->Temperature = 0.0;
    }
    return true;
}



/***********************************************************************************************************************************************************************************
 * /brief   Function to print the conversion results to the Serial interface
 * /param	measured	 Pointer to the structure where the conversion results are stored.		
***********************************************************************************************************************************************************************************/
void TH06_print_temp_and_humidity (TH06_DATA * print)
{
	Serial.print("Temperature: ");
	Serial.print((print->Temperature), DEC);
	Serial.print(", Humidity: ");
	Serial.print((print->Humidity), DEC);
	Serial.println();
	Serial.flush();
}

















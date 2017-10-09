/*
 * movement.cpp
 *
 * Created: 3-10-2017 14:00:42
 *  Author: adri
 */ 
#include <avr/io.h>
#include <Arduino.h>
#include "demoboard.h"

/*
	@brief	
	Initializes the movement sensor on the Nexus demoboard, so it generates Pin Change Interrupts when the sensor pulls the A0 pin low.
	As the Pin change interrupt is unidirectional there is no rising or falling edge detection. 
	
	@warning
	Since the hardware pull-up on the demoboard is used to give the input pin a default state, don't use this code without the demoboard 
	or enable an internal pull-up to prevent erratic behavior and higher power consumption without a default pin state.
*/
void movement_sensor_init (void)
{
	// Enable the pull-up like this, even tough it's already provided by the hardware, but might be useful if the nexus is removed from the demoboard to prevent erratic behaviour without a defined pin state.
	pinMode(MOVEMENT, INPUT_PULLUP);
	
	// Enable the movement sensor to trigger an Pin Change Interrupt request
	PCMSK1 |= 0x01;
	
	// Enable Pin Change Interrupt 1
	PCICR |= 0x02;
}

/*
	@brief	
	Measure and calculate the supply voltage of the atmega328p, by setting the supply voltage as ADC reference and then measure the internal
	1.1V reference in the atmega 328p. The supply voltage can then be calculated with the following calculation: 
	VCC = ((Vref * 2^ADC_resolution) / ADC_result).
	
	@warning
	In order for this function to work, the Arduino function analogRead() must be modified so it will be able to measure the reference voltage.
	So open an explorer and go to C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\wiring_analog.c and open it with an text 
	editor that has administrator privileges and add the if statement shown below on the first piece of code with all the #if definitions:
		
	uint8_t low, high;
	// Only do the pin to channel conversion when the seventh bit of pin is not set
	if((pin & 0x80) == 0x00)
	{
		#if defined(analogPinToChannel)
		#if defined(__AVR_ATmega32U4__)
		if (pin >= 18) pin -= 18; // allow for channel or pin numbers
		#endif
		pin = analogPinToChannel(pin);
		#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		if (pin >= 54) pin -= 54; // allow for channel or pin numbers
		#elif defined(__AVR_ATmega32U4__)
		if (pin >= 18) pin -= 18; // allow for channel or pin numbers
		#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
		if (pin >= 24) pin -= 24; // allow for channel or pin numbers
		#else
		if (pin >= 14) pin -= 14; // allow for channel or pin numbers
		#endif
	}
	
	This if statement bypasses the analogPinToChannel conversion and will allow writing values to the register directly if the pin variable
	has the 7th bit set to 1. You also need to modify 0x07 to 0x0F in this next  piece of code:
	
		#if defined(ADMUX)
		#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		ADMUX = (analog_reference << 4) | (pin & 0x07);
		#else
		ADMUX = (analog_reference << 6) | (pin & 0x07);
		#endif
		#endif
	
	to
	
		#if defined(ADMUX)
		#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		ADMUX = (analog_reference << 4) | (pin & 0x0F);
		#else
		ADMUX = (analog_reference << 6) | (pin & 0x0F);
		#endif
		#endif
	
	Then save the file and recompile your project.
	
*/
double read_supply_voltage(void) 
{
	double vcc;
	
	
	/*
		A modification to the Arduino analogRead function is required!! See the warning statement above!
	*/
	
	// Sample the reference voltage with the modified analog read function
	vcc = 1126.4 / (double) (analogRead(0x8E));
	return vcc; // Supply voltage in millivolts
}

/*
*/
void demoboard_enable_button_wakeup (void)
{
	// Enable the Pull_UPs on the buttons, so 
	pinMode(BUTTON1, INPUT_PULLUP);
	pinMode(BUTTON2, INPUT_PULLUP);
		
	// Enable the buttons to trigger an Pin Change Interrupt request
	PCMSK1 |= 0x06;
		
	// Enable Pin Change Interrupt 1
	PCICR |= 0x02;
}


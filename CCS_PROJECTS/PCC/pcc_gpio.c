#include "pcc_gpio.h"

void Init_GPIO()
{
	P1DIR |= RED_LED;
	P1DIR |= GREEN_LED;

	P1OUT = 0x00;
}

void sendGPIO(volatile unsigned char* portRef, unsigned char bit, unsigned short value)
{
	if (portRef == NULL)
		return;

	//Perform Operation Based on Value
	switch (value)
	{
		case LOGIC_HIGH:
			*portRef |=  bit;
		break;
		case LOGIC_LOW:
			*portRef &= ~bit;
		break;
		case LOGIC_TOGGLE:
			*portRef ^=  bit;
		break;
	}

}

void toggleGreen()
{
	setLED(GREEN_LED, LOGIC_TOGGLE);
}

void toggleRed()
{
	//P1OUT ^= 0x01;
	setLED(RED_LED, LOGIC_TOGGLE);
}

void toggleAll()
{
	toggleRed();
	toggleGreen();
}

void setLED(unsigned char LED, unsigned char value)
{
	sendGPIO(PORT1_REF, LED, value);
}

void setLEDs(unsigned char value)
{
	setLED(RED_LED, value);
	setLED(GREEN_LED, value);
}

//Blink LEDs for Debugging
void blinkLEDs(unsigned int numTimes)
{
	volatile unsigned int i = 0;

	if (numTimes > 5)
		numTimes = 5; //We Don't have all day man

	setLEDs(OFF);
	for (i = 0; i < numTimes * 2; i++)
	{
		setLEDs(LOGIC_TOGGLE);
		_delay_cycles(MCU_CLOCK / 2); //half second delay
	}
}

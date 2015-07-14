#include "pcc_gpio.h"

void Init_GPIO()
{
	P1DIR |= ALL_BITS_MASK;
	P2DIR |= ALL_BITS_MASK;

	P1OUT = 0x00;
	P2OUT = 0x00;
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


//LED FUNCTIONS
void Init_LED()
{
	P1DIR |= 0x33;//Bit1 and Bit6 High
	P1OUT = 0x00;
}
void toggleGreen()
{
	//P2OUT ^= 0x04;
	sendGPIO(PORT2_REF, BIT3, LOGIC_TOGGLE);
}

void toggleYellow()
{
	//P1OUT ^= 0x32;
	sendGPIO(PORT1_REF, BIT6, LOGIC_TOGGLE);
}

void toggleRed()
{
	//P1OUT ^= 0x01;
	sendGPIO(PORT1_REF, BIT0, LOGIC_TOGGLE);
}

void toggleAll()
{
	toggleRed();
	toggleGreen();
	toggleYellow();
}

void setLED(unsigned char LED, unsigned char value)
{
	sendGPIO(PORT1_REF, LED, value);
}

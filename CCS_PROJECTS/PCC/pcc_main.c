#include <msp430g2553.h>
#include "pcc_uart_comm.h"
#include "pcc_cmd_control.h"
#include "pcc_gpio.h"

#define NO_INTERRUPT_PENDING 0x00
#define INTERRUPT_PENDING    0x01

#define LOW2HIGH_TRANSTION   0x01
#define HIGH2LOW_TRANSTION   0x10

#define INTERRUPT_DISABLED   0x00
#define INTERRUPT_ENABLED    0x10

#define INPUT_DIRECTION      0x00
#define OUTPUT_DIRECTION     0x01

#define IFG_BIT 0x10

//To Clean up Main()
void Initialize_PCC();

void main(int argc, char** argv)
{
	PCC_Command_Type lastCmd, currentCmd;

	//Configure and Initialize the MSP430 for PCC Commands
	Initialize_PCC();
	lastCmd    = PCC_CMD_NONE;
	currentCmd = PCC_CMD_NONE;

	//Test Each Possible Servo Command
	ServoTest();

	//Listen and Process Commands
	while(1)
	{
		//Get the Command from the UART
		currentCmd = INCHAR_UART();
		OUTCHAR_UART(currentCmd);

		//Keep Track of the commands we send for processing
		lastCmd = processCommand(currentCmd, lastCmd);
	}
}

//int main(void)
//{
//    Init_UART();
//
//    while (1)
//    {
//        OUTCHAR_UART(INCHAR_UART());
//    }
//
//    return 0;
//}

//Initialize Power Chair Controller Software
void Initialize_PCC()
{
	const char* initStatement = "PCC Initialization Complete, Listening for PCC Commands";

	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

	Init_UART();
	Init_GPIO();
	initCmdControl();

	println(initStatement);
}

////-------------SOME DEBUG STUFF HERE LEAVE ME ALONE-------------
//void main (void)
//{
//
//	volatile unsigned int  i;
//	unsigned char c = 0x00;
//
//
//	Initialize_PCC();
//
//	// Main loop
//	while (1)
//	{
//
//		c = INCHAR_UART();
//	    OUTCHAR_UART(c);
//
//		if (1)
//		{
//			TACCR1 =servo_lut[0];
//		}
//
//		if (c >= '0' && c <= '9')
//		{
//			unsigned short index = (unsigned short) (c - 0x30) * 10;
//
//			// Go forward
//			TA0CCR1 = servo_lut[index];
//		}
//		else if (c == 'a')
//		{
//			// Go backward
//			TA0CCR1 = servo_lut[120];
//		}
//		else if (c == 'b')
//		{
//			// Go backward
//			TA0CCR1 = servo_lut[140];
//		}
//
//		else if (c == 'c')
//		{
//			// Go backward
//			TA0CCR1 = servo_lut[160];
//		}
//		else if (c == 'd')
//		{
//			// Go backward
//			TA1CCR1 = servo_lut[179];
//		}
//		else if (c == 'f')
//		{
//			// Go backward
//			TA1CCR1 = servo_lut[179];
//		}
//		else if (c == 'g')
//		{
//			// Go backward
//			TA1CCR1 = servo_lut[0];
//		}
//		else if (c == 's')
//		{
//			TA1CCR1 = PWM_OFF;//stop Servo
//		}
//	}
//}

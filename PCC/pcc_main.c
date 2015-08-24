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

//-------------SOME DEBUG STUFF HERE LEAVE ME ALONE-------------
//void main (void)
//{
//
//	volatile unsigned int  i;
//	volatile unsigned char c;
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
//		if (c == 'f')
//		{
//			// Go forward
//			TA0CCR1 = servo_lut[179];
//			__delay_cycles(2000000);
//			TACCR1 = PWM_OFF;//stop Servo
//		}
//		else if (c == 'b')
//		{
//			// Go backward
//			TA0CCR1 = servo_lut[0];
//			__delay_cycles(1000000);
//		}
//		else if (c == 'c')
//		{
//			// Go backward
//			TA0CCR1 = servo_lut[90];
//			__delay_cycles(1000000);
//
//		}
//
//		else if (c == 's')
//		{
//			TACCR1 = PWM_OFF;//stop Servo
//		}
//		else
//		{
//			//TACCR1 = PWM_OFF;//stop Servo
//		}
//	}
//}

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

const char* initStatement = "PCC Initialize Complete. Waiting for Commands...";

void main(int argc, char** argv)
{
	volatile unsigned char c;

	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

	Init_UART();
	Init_GPIO();
	println(initStatement);

	while(1)
	{
		c = INCHAR_UART();
		OUTCHAR_UART(c);

		processCommand(c);
		//sleep(DEFAULT_DELAY);
	}
}

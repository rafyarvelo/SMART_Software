/*
 * brs_c_uart.c
 *
 *  Created on: Sep 25, 2015
 *      Author: Rafael
 */

#include "brs_c_uart.h"

volatile uint8_t RECEIVE_BUFFER[MAX_BUFFER_SIZE];

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

#ifdef UART_INTERRUPT
//*****************************************************************************
//
// Initialize the UART for interrupt enabled responses
//
//*****************************************************************************
void Init_UART()
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable the GPIO pins for the LED (PF2).
    //
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void UARTIntHandler(void)
{
    uint32_t ui32Status;
    char c;
    static uint16_t charsReceived = 0;

    //
    // Get the interrrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
    	c = ROM_UARTCharGetNonBlocking(UART0_BASE);
        ROM_UARTCharPutNonBlocking(UART0_BASE, c);

    	//Buffer Received Character
    	RECEIVE_BUFFER[charsReceived++] = c;

    	//Check Msg Id every 8 bytes
    	if (charsReceived > 0 && charsReceived % 8 == 0)
    	{
    		CheckMessageID((volatile uint8_t*) &RECEIVE_BUFFER[charsReceived - 8]);

    		//The call to Check Message ID should of read in what we needed, reset the buffer
    		charsReceived = 0;
    	}

        //Blink the LED for Status
        LED_Blink(1);
    }
}

#else//FreeRTOS UART Task

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTSend().
//
//*****************************************************************************
void ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

#endif //UART_INTERRUPT

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

//*****************************************************************************
//
// Blink the LED for Debugging/Status
//
//*****************************************************************************
void LED_Blink(uint16_t delay)
{
    //
    // Blink the LED to show a character transfer is occuring.
    //
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

    //
    // Delay for "delay" milliseconds.  Each SysCtlDelay is about 3 clocks.
    //
    SysCtlDelay(SysCtlClockGet() / ((1000 * 3) * delay));

    //
    // Turn off the LED
    //
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
}

//*****************************************************************************
//
// Retrieve a string from the UART, return bytes actually read
//
//*****************************************************************************
uint16_t UARTReceive(volatile uint8_t *pui8Buffer, uint32_t ui32Count)
{
	uint16_t bytesReceived = 0;

	//
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(UART0_BASE) && ui32Count--)
    {
        //
        // Read the next character from the UART and put it in the buffer
        //
    	*pui8Buffer++ = ROM_UARTCharGetNonBlocking(UART0_BASE);

        //Blink the LED for Status
        LED_Blink(1);

        //Increment the Number of Bytes Received
    	bytesReceived++;
    }

    return bytesReceived;
}

//*****************************************************************************
//
// Return true  (1) if there is a BCI Message ready in the UART, otherwise
// return false (0)
//
//*****************************************************************************
int BCIMessageAvailable()
{
	uint32_t msgIdSize        = sizeof(MSG_ID_Type);
	volatile uint8_t *pMsgId  = malloc(msgIdSize);

	//Check for a Message ID
	UARTReceive(pMsgId, msgIdSize);

	//Return True if a message Id was found
	if (pMsgId != NULL && *pMsgId == BRS2BCI_MSG_ID)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//*****************************************************************************
//
// Read Incoming BCI Message
//
//*****************************************************************************
TM_Frame_t* ReadBCI2BRSMsg()
{
	TM_Frame_t* ptr = createTMFrame();

	//Get the Message
	UARTReceive((volatile uint8_t*) ptr, sizeof(TM_Frame_t));

	//return Message
	return ptr;
}

//*****************************************************************************
//
// Send a BRS Frame through the UART
//
//*****************************************************************************
void SendBRSFrame(BRS_Frame_t* pFrame)
{
	UARTSend((const uint8_t*) pFrame, sizeof(BRS_Frame_t));
}

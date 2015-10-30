/*
 * brs_c_uart.c
 *
 *  Created on: Sep 25, 2015
 *      Author: Rafael
 */

#include "brs_c_uart.h"
#include "utils/uartstdio.h"

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

//*****************************************************************************
//
// Configure the UARTs and their pins.  This must be called before UARTSend().
//
//*****************************************************************************
void ConfigureUARTs(void)
{
	//The 5 Five UARTs we will need for our application
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable UARTs
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinConfigure(GPIO_PC4_U1RX);
    ROM_GPIOPinConfigure(GPIO_PC5_U1TX);
    ROM_GPIOPinConfigure(GPIO_PD6_U2RX);
    ROM_GPIOPinConfigure(GPIO_PD7_U2TX);
    ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
    ROM_GPIOPinConfigure(GPIO_PC7_U3TX);
    ROM_GPIOPinConfigure(GPIO_PE4_U5RX);
    ROM_GPIOPinConfigure(GPIO_PE5_U5TX);
    ROM_GPIOPinConfigure(GPIO_PD4_U6RX);
    ROM_GPIOPinConfigure(GPIO_PD5_U6TX);

    //
    // Configure Pin Types
    //
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), UART_BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), UART_BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), UART_BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), UART_BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), UART_BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));

    //
    // Initialize the CONSOLE UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(UART_ID uartID, const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPutNonBlocking(uartID, *pui8Buffer++);
    }
}

//*****************************************************************************
//
// Retrieve a string from the UART, return bytes actually read
//
//*****************************************************************************
uint16_t UARTReceive(UART_ID uartID, volatile uint8_t *pui8Buffer, uint32_t ui32Count)
{
	uint16_t bytesReceived = 0;

	//
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(uartID) && ui32Count--)
    {
        //
        // Read the next character from the UART and put it in the buffer
        //
    	*pui8Buffer++ = ROM_UARTCharGetNonBlocking(uartID);

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
    MsgIdType msgId;

	//Check for a Message ID
	UARTReceive(BCI_UART, (volatile uint8_t*) &msgId, MSG_ID_SIZE);

	//Return True if a message Id was found
	if (checkMsgID(msgId, BRS2BCI_MSG_ID))
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
	UARTReceive(BCI_UART, (volatile uint8_t*) ptr, sizeof(TM_Frame_t));

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
	UARTSend(BCI_UART, (const uint8_t*) pFrame, sizeof(BRS_Frame_t));
}

//*****************************************************************************
//
// Retrieve a Bluetooth Frame from the UART
//
//*****************************************************************************
BluetoothFrame_t* ReadBluetoothFrame()
{
	BluetoothFrame_t* pFrame = createBluetoothFrame();

	UARTReceive(BT_UART, (volatile uint8_t*) pFrame, sizeof(BluetoothFrame_t));

	return pFrame;
}

//*****************************************************************************
//
// Check for a Bluetooth Frame in the UART
//
//*****************************************************************************
int BluetoothFrameAvailable()
{
	return ROM_UARTCharsAvail(BT_UART);
}

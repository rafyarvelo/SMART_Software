/*
 * brs_c_uart.h
 *
 *  Created on: Sep 25, 2015
 *      Author: Rafael
 */

#ifndef BRS_C_UART_H_
#define BRS_C_UART_H_

//Necessary Libraries
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "../../smart_data_types.h"

typedef enum UART_ID
{
	CONSOLE_UART = UART0_BASE, //Console I/0 UART
	BCI_UART     = UART1_BASE, //BCI Comm UART
	GPS_UART     = UART2_BASE, //GPS Sensor UART
	BT_UART      = UART3_BASE, //Bluetooth UART
	USF_UART     = UART5_BASE, //Ultrasonic Range Finder - Front
	USR_UART     = UART6_BASE  //Ultrasonic Range Finder - Back
} UART_ID;

#define UART_BAUD_RATE 112500

//*****************************************************************************
//
// Configure the UARTs and their pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUARTs(void);

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(UART_ID uartID, const uint8_t *pui8Buffer, uint32_t ui32Count);

//*****************************************************************************
//
// Retrieve a string from the UART, return bytes actually read
//
//*****************************************************************************
uint16_t UARTReceive(UART_ID uartID, volatile uint8_t *pui8Buffer, uint32_t ui32Count);

//*****************************************************************************
//
// Return true  (1) if there is a BCI Message ready in the UART, otherwise
// return false (0)
//
//*****************************************************************************
int BCIMessageAvailable();

//*****************************************************************************
//
// Read Incoming BCI Message from UART
//
//*****************************************************************************
TM_Frame_t* ReadBCI2BRSMsg();

//*****************************************************************************
//
// Send a BRS Frame through the UART
//
//*****************************************************************************
void SendBRSFrame(BRS_Frame_t* pFrame);

//*****************************************************************************
//
// Check for a Bluetooth Frame in the UART
//
//*****************************************************************************
int BluetoothFrameAvailable();

//*****************************************************************************
//
// Retrieve a Bluetooth Frame from the UART
//
//*****************************************************************************
BluetoothFrame_t* ReadBluetoothFrame();

#endif /* BRS_C_UART_H_ */

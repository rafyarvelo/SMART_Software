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
	GPS_UART     = UART1_BASE, //GPS Sensor UART

	//Hack For Now
#ifndef BRS_DEBUG
	BCI_UART     = UART2_BASE, //BCI Comm UART
#else
	BCI_UART     = UART0_BASE, //BCI Comm UART
#endif

	BT_UART      = UART3_BASE, //Bluetooth UART
	USF_UART     = UART5_BASE, //Ultrasonic Range Finder - Front
	USR_UART     = UART6_BASE  //Ultrasonic Range Finder - Back
} UART_ID;

//Default BAUD_RATE for all the boards
#define BAUD_RATE      9600

//FOR GPS SENSORS
#define GPS_NAV_MSG_ID "$GPRMC"
#define GPS_DATA_DELIM ','

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
// Retrieve a string from the UART until a delimeter is found, return bytes actually read
//
//*****************************************************************************
uint16_t UARTReceiveUntil(UART_ID uartID, volatile uint8_t *pui8Buffer, uint8_t delim);

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
void ReadBCI2BRSMsg(TM_Frame_t* pFrame);

//*****************************************************************************
//
// Read Incoming GPS Data, return TRUE if data is available
//
//*****************************************************************************
int ReadGPSData(GPS_Data_t* pData);

//*****************************************************************************
//
// Read Range Finder data
//
//*****************************************************************************
void ReadUSData(US_Data_t* pData);

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
// Check for a GPS Frame in the UART
//
//*****************************************************************************
int GPSDataAvailable();

//*****************************************************************************
//
// Check if the
//
//*****************************************************************************
int TMFrameRequested();

//*****************************************************************************
//
// Retrieve a Bluetooth Frame from the UART
//
//*****************************************************************************
void ReadBluetoothFrame(BluetoothFrame_t* pFrame);

//*****************************************************************************
//
// Send a TM Frame through the Bluetooth Module
//
//*****************************************************************************
void SendTMFrame(TM_Frame_t* pFrame);

#endif /* BRS_C_UART_H_ */

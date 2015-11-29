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
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "../../smart_data_types.h"
#include "brs_c_sensor_formats.h"

typedef enum UART_ID
{

	CONSOLE_UART = UART0_BASE, //Console I/0 UART
	GPS_UART     = UART1_BASE, //GPS Sensor UART
	USR_UART     = UART2_BASE,  //Ultrasonic Range Finder - Back

	BCI_UART     = UART0_BASE, //BCI Comm UART

	BT_UART      = UART3_BASE, //Bluetooth UART
	USF_UART     = UART5_BASE  //Ultrasonic Range Finder - Front
} UART_ID;

//Default BAUD_RATE for all the boards
#define BAUD_RATE 9600

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
uint16_t UARTReceiveUntil(UART_ID uartID, char *pui8Buffer, char delim, uint32_t maxSize);

//*****************************************************************************
//
// Retrieve all the delimeted words until a CRLF is seen and place in a buffer.
// Return the number of words read
//
//*****************************************************************************
uint16_t UARTReadDelimetedLine(UART_ID uartID, char** pui8DoubleBuffer, char delim);

//*****************************************************************************
//
// Retrieve a pointer to a substring inside a delimeted string, return NULL
// if offset is invalid or delim not found. offset is the WORD INDEX of the
// desired delimeted word
//
//*****************************************************************************
char* extractDelimetedString(char* delimetedString, uint32_t size, uint32_t offset, char delim, uint32_t* wordSize);

//*****************************************************************************
//
// Convert ASCII Character to Hexadecimal
//
//*****************************************************************************
uint8_t ASCII2HEX(char asciiChar);

//*****************************************************************************
//
// Convert ASCII word to Number, read until '\0' is seen
//
//*****************************************************************************
uint32_t ASCII2UINT(const uint8_t* pui8buffer, uint32_t length);

//*****************************************************************************
//
// Convert ASCII word to Number, read until count or '\0' is seen, return status
//
//*****************************************************************************
uint8_t ASCII2FLOAT(const uint8_t* pui8buffer, uint32_t length, float* pFloat);

//*****************************************************************************
//
// Read Incoming BCI Message from UART
//
//*****************************************************************************
uint8_t ReadBCI2BRSMsg(TM_Frame_t* pFrame);

//*****************************************************************************
//
// Read Incoming GPS Data, return TRUE if data is available
//
//*****************************************************************************
uint8_t ReadGPSData(GPS_Data_t* pData);

//*****************************************************************************
//
// Read Range Finder data
//
//*****************************************************************************
uint8_t ReadUSData(US_Data_t* pData);

//*****************************************************************************
//
// Send a BRS Frame through the UART
//
//*****************************************************************************
void SendBRSFrame(BRS_Frame_t* pFrame);

//*****************************************************************************
//
// Send a TM Frame through the Bluetooth Module
//
//*****************************************************************************
void SendTMFrame(TM_Frame_t* pFrame);

#endif /* BRS_C_UART_H_ */

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
#include "brs_c_message_constants.h"

//*****************************************************************************
//
// Initialize the UART
//
//*****************************************************************************
void Init_UART();


//*****************************************************************************
//
// Blink the LED for Debugging/Status
//
//*****************************************************************************
void LED_Blink(uint16_t delay);


//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void UARTIntHandler(void);


//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);


//*****************************************************************************
//
// Retrieve a string from the UART, return bytes actually read
//
//*****************************************************************************
uint16_t UARTReceive(volatile uint8_t *pui8Buffer, uint32_t ui32Count);


//*****************************************************************************
//
// Check for Messages coming in from the UART and handle them appropriately.
// Parameter should be the base address of a received character buffer
//
//*****************************************************************************
void CheckMessageID(volatile uint8_t* addr);


//*****************************************************************************
//
// Read Incoming BCI Message
//
//*****************************************************************************
void ReadBCI2BRSMsg(volatile uint8_t *pui8Buffer, uint32_t ui32Count);


//*****************************************************************************
//
// Read Incoming Mobile Device Message
//
//*****************************************************************************
void ReadMD2BRSMsg(volatile uint8_t *pui8Buffer, uint32_t ui32Count);

#endif /* BRS_C_UART_H_ */

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

//*****************************************************************************
//
// Blink the LED for Debugging/Status
//
//*****************************************************************************
void LED_Blink(uint16_t delay);

#ifdef UART_INTERRUPT
//*****************************************************************************
//
// Initialize the UART for interrupt enabled responses. This will be our
// Secondary method of UART Communication if the UART Task is not responsive
//
//*****************************************************************************
void Init_UART();

//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
void UARTIntHandler(void);

#else //Use FreeRTOS UART Task

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void ConfigureUART(void);

#endif //UART_INTERRUPT

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

#endif /* BRS_C_UART_H_ */

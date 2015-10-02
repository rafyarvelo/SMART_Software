/*
 * brs_c_bluetooth.h
 *
 *  Created on: Sep 29, 2015
 *      Author: Rafael
 */

#ifndef BRS_C_BLUETOOTH_H_
#define BRS_C_BLUETOOTH_H_

#include "../../smart_data_types.h"
//*****************************************************************************
//
// Send a string to the Bluetooth Module.
//
//*****************************************************************************
void BluetoothSend(const uint8_t *pui8Buffer, uint32_t ui32Count);

//*****************************************************************************
//
// Retrieve a string from the Bluetooth Module, return bytes actually read
//
//*****************************************************************************
uint16_t BluetoothReceive(volatile uint8_t *pui8Buffer, uint32_t ui32Count);

//*****************************************************************************
//
// Send a TM Frame through the Bluetooth Module
//
//*****************************************************************************
void SendTMFrame(TM_Frame_t* pFrame);

#endif /* BRS_C_BLUETOOTH_H_ */

//*****************************************************************************
//
// led_task.c - A simple flashing LED task.
//
// Copyright (c) 2012-2015 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.1.71 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "brs_c_led.h"
#include "brs_c_data_bridge_task.h"
#include "../../smart_data_types.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//*****************************************************************************
//
// The stack size for the Data Bridge task.
//
//*****************************************************************************
#define DATABRIDGETASKSTACKSIZE        128         // Stack size in words
#define DATABRIDGE_QUEUE_SIZE          5           // Queue size in messages

//*****************************************************************************
//
// Queue Handles Needed
//
//*****************************************************************************
extern xSemaphoreHandle g_pBluetoothReceiveQueue;
extern xSemaphoreHandle g_pSensorDataQueue;
extern xSemaphoreHandle g_pUARTSendQueue ;


static void DataBridgeTask(void *pvParameters)
{
    portTickType ui32WakeTime;
    SensorData_t     sensorData;
    BluetoothFrame_t btFrame;
    BRS_Frame_t      BRSFrameToSend;
    int brsFrameInitialized   = FALSE;
    int sensorDataAvailable   = FALSE;
    int remoteDataAvailable   = FALSE;

    // Get the current tick count.
    ui32WakeTime = xTaskGetTickCount();

    // Loop forever.
    while(1)
    {
    	//Check for Sensor Data
    	if (xQueueReceive(g_pSensorDataQueue, &sensorData, 0) == pdPASS)
    	{
    		sensorDataAvailable = TRUE;
    	}

    	//Check for Remote Data
    	if (xQueueReceive(g_pBluetoothReceiveQueue, &btFrame, 0) == pdPASS)
		{
			remoteDataAvailable = TRUE;
		}

    	//Create BRS Frame from Received Data and Reset Flags
    	if (sensorDataAvailable || remoteDataAvailable)
    	{
    		//Initialize Message ID
    		memcpy(&BRSFrameToSend.MsgId, BRS2BCI_MSG_ID, MSG_ID_SIZE);

    		//Sensor Data
			memcpy(&BRSFrameToSend.sensorData, &sensorData, sizeof(SensorData_t));
			sensorDataAvailable = FALSE;

			//Remote Data
			memcpy(&BRSFrameToSend.remoteCommand, &btFrame.remoteCommand, sizeof(PCC_Command_Type));
			remoteDataAvailable = FALSE;

			brsFrameInitialized = TRUE;
    	}

    	// Pass the frame to the UART Task.
		if (brsFrameInitialized == TRUE)
		{
			if(xQueueSend(g_pUARTSendQueue, &BRSFrameToSend, portMAX_DELAY) != pdPASS)
			{
				// Error. The queue should never be full. If so print the
				// error message on UART and wait for ever.
				UARTprintf("\nQueue full. This should never happen.\n");
				while(1)
				{
					BlinkLED(TIVA_RED_LED, 5);
				}
			}

			//Grab a new Frame next time
			brsFrameInitialized = FALSE;
		}

    	//Wait the required amount of time
    	vTaskDelayUntil(&ui32WakeTime, DATA_BRIDGE_TASK_DELAY / portTICK_RATE_MS);//Do Stuff
    }

}

//*****************************************************************************
//
// Initializes the Data Bridge task.
//
//*****************************************************************************
uint32_t DataBridgeTaskInit(void)
{
    #ifdef VERBOSE
    UARTprintf("Initializing Data Bridge Task...\n");
    #endif
    //
    // Create the Data Bridge task.
    //
    if(xTaskCreate(DataBridgeTask, (signed portCHAR *)"DataBridge", DATABRIDGETASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_DATA_BRIDGE_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}

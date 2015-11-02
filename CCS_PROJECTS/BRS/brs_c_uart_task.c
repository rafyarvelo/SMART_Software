//*****************************************************************************
//
// brs_c_uart_task.c - Implementation for the UART task.
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
#include "brs_c_uart_task.h"
#include "../../smart_data_types.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "brs_c_uart.h"
#include "brs_c_led.h"
#include "queue.h"
#include "semphr.h"

//*****************************************************************************
//
// The stack size for the UART task.
//
//*****************************************************************************
#define UARTTASKSTACKSIZE        128         // Stack size in words
#define UART_QUEUE_SIZE          5           // Queue size in messages

//*****************************************************************************
//
// Queue Handles Needed
//
//*****************************************************************************
extern xQueueHandle g_pBluetoothSendQueue;
       xQueueHandle g_pUARTSendQueue;

//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
extern xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// This task communicates with the BCI Processor via UART
//
//*****************************************************************************
static void UARTTask(void *pvParameters)
{
    portTickType  ui32WakeTime;
    BRS_Frame_t BRSFrameToSend;
    TM_Frame_t  receivedTMFrame;
    int         frameReadyToSend = FALSE;

    //
    // Get the current tick count.
    //
    ui32WakeTime = xTaskGetTickCount();

    //Initialize the TM Frame
    memset(&receivedTMFrame, 0, sizeof(TM_Frame_t));

    //
    // Loop forever.
    //
    while(1)
    {
		#ifdef BRS_DEBUG

    	//Generate Random TM Frame
    	memcpy(&receivedTMFrame.MsgId, BCI2BRS_MSG_ID, sizeof(MsgIdType));
    	receivedTMFrame.lastCommand = "fblr"[(rand() % 4)];

    	//Send the Frame to the Bluetooth Task
		xQueueSend(g_pBluetoothSendQueue, &receivedTMFrame , portMAX_DELAY);

		//Send BRS Message
		if (xQueueReceive(g_pUARTSendQueue, &BRSFrameToSend, 0) == pdPASS)
		{
			memcpy(&BRSFrameToSend.MsgId, BRS2BCI_MSG_ID, MSG_ID_SIZE);
			xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
			SendBRSFrame(&BRSFrameToSend);
			xSemaphoreGive(g_pUARTSemaphore);
			BlinkLED(TIVA_GREEN_LED, 1); //Blink Send Status
		}

		#else //Actually Send the Data

    	//Try and Get a TM Frame from the UART
    	ReadBCI2BRSMsg(&receivedTMFrame);

    	//If the Frame was Good, put it in the Queue
    	if (checkMsgID(receivedTMFrame.MsgId, BCI2BRS_MSG_ID))
    	{
			//Blink Receive Status
			BlinkLED(TIVA_GREEN_LED, 1);

    		//Send the Frame to the Bluetooth Task
    		xQueueSend(g_pBluetoothSendQueue, &receivedTMFrame , portMAX_DELAY);

    		//Reset the Message ID so we check it next time
    		memset(&receivedTMFrame.MsgId,0, sizeof(MsgIdType));
    	}

    	//Send The BRS Message through the UART if it is Available
    	if (!frameReadyToSend && xQueueReceive(g_pUARTSendQueue, &BRSFrameToSend, 0) == pdPASS)
    	{
    		frameReadyToSend = TRUE;
    	}

    	if (frameReadyToSend)
    	{
    		//Make Sure the Message ID Is Correct
            memcpy(&BRSFrameToSend.MsgId, BRS2BCI_MSG_ID, MSG_ID_SIZE);

    		//Send Message and Block concurrent access using semaphore
            xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
            SendBRSFrame(&BRSFrameToSend);
            xSemaphoreGive(g_pUARTSemaphore);

    		//Reset Frame Ready Flag
    		frameReadyToSend = FALSE;
    	}

		#endif

    	vTaskDelayUntil(&ui32WakeTime, UART_TASK_DELAY / portTICK_RATE_MS);
    }
}

//*****************************************************************************
//
// Initializes the UART task.
//
//*****************************************************************************
uint32_t UARTTaskInit(void)
{
    #ifdef VERBOSE
    UARTprintf("\nInitializing UART Task...\n");
    #endif
    //
    // Create a queue for sending messages to the LED task.
    //
    g_pUARTSendQueue = xQueueCreate(UART_QUEUE_SIZE,BRS2BCI_SIZE);

    //
    // Create the LED task.
    //
    if(xTaskCreate(UARTTask, (signed portCHAR *)"UART", UARTTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_UART_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}

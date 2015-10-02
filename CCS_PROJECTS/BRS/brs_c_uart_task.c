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
// This task communicates with the BCI Processor via UART
//
//*****************************************************************************
static void UARTTask(void *pvParameters)
{
    portTickType  ui32WakeTime;
    TM_Frame_t*  pReceivedTMFrame = NULL;
    BRS_Frame_t* pBRSFrameToSend  = NULL;

    //
    // Get the current tick count.
    //
    ui32WakeTime = xTaskGetTickCount();

    //Initialize the TM Frame

    //
    // Loop forever.
    //
    while(1)
    {
    	//Receive BCI Message if it is Available
    	if (BCIMessageAvailable())
    	{
        	//Receive TM Frame from UART
    		pReceivedTMFrame = ReadBCI2BRSMsg();

    		//Send the Frame to the Bluetooth Task
    		xQueueSend(g_pBluetoothSendQueue,&pReceivedTMFrame, portMAX_DELAY);
    	}

    	//Send The BRS Message through the UART if it is Available
    	if (xQueueReceive(g_pUARTSendQueue, &pBRSFrameToSend, 0) == pdPASS)
    	{
    		SendBRSFrame(pBRSFrameToSend);
    		BlinkLED(GREEN_LED, 1);
    	}

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
    UARTprintf("\nInitializing UART Task...\n");

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
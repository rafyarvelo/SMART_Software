#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "brs_c_bt_task.h"
#include "brs_c_bluetooth.h"
#include "../../smart_data_types.h"
#include "../PCC/power_chair_command_constants.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "brs_c_led.h"
#include "semphr.h"

//*****************************************************************************
//
// The stack size for the UART task.
//
//*****************************************************************************
#define BLUETOOTHTASKSTACKSIZE        128         // Stack size in words
#define BLUETOOTH_QUEUE_SIZE          5           // Queue size in messages

//*****************************************************************************
//
// Queue Handles Needed
//
//*****************************************************************************
xQueueHandle g_pBluetoothSendQueue;
xQueueHandle g_pBluetoothReceiveQueue;

static void BluetoothTask(void *pvParameters)
{
    portTickType     ui32WakeTime;
    BluetoothFrame_t* pReceivedBTFrame = malloc(sizeof(BluetoothFrame_t));
    TM_Frame_t*       pFrameToSend     = NULL;
    int               btFrameReceived  = FALSE;

    const uint16_t pcc_cmds_SIZE = 4;
    PCC_Command_Type pcc_cmds[] =
	{
			PCC_FORWARD,
			PCC_BACKWARD,
			PCC_RIGHT,
			PCC_LEFT
	};

    //
    // Get the current tick count.
    //
    ui32WakeTime = xTaskGetTickCount();

    //Initialize the Receive Frame and TM Frame
    memset(pReceivedBTFrame, 0, sizeof(BluetoothFrame_t));

    // Loop forever.
    while(1)
    {
    	//=========Get Bluetooth Data==========
    	//Generate Debug Data
		#ifdef DEBUG_ONLY
    		pReceivedBTFrame->remoteCommand = pcc_cmds[rand() % pcc_cmds_SIZE];
    		btFrameReceived = TRUE;
    	//Actually Get Data
		#else

		#endif

    	if (btFrameReceived == TRUE)
    	{
    		BlinkLED(BLUE_LED, 1);
			//Send the Data to the Data Bridge
			if(xQueueSend(g_pBluetoothReceiveQueue, &pReceivedBTFrame, portMAX_DELAY) != pdPASS)
			{
				// Error. The queue should never be full. If so print the
				// error message on UART and wait for ever.
				UARTprintf("\nQueue full. This should never happen.\n");
				while(1)
				{
				}
			}
    		btFrameReceived = FALSE;
    	}

        //=====================================

		//========Send Bluetooth Data==========

		//Get the TM Frame from the UART Task and Send it
    	if (xQueueReceive(g_pBluetoothSendQueue, &pFrameToSend, 0) == pdPASS)
    	{
    		SendTMFrame(pFrameToSend);
    	}

        //=====================================

    	vTaskDelayUntil(&ui32WakeTime, BT_TASK_DELAY / portTICK_RATE_MS);//Do Stuff
    }


}

//*****************************************************************************
//
// Initializes the Bluetooth task.
//
//*****************************************************************************
uint32_t BluetoothTaskInit(void)
{
    UARTprintf("Initializing Bluetooth Task...\n");

    //
    // Create a queue for sending/receiving messages
    //
    g_pBluetoothSendQueue    = xQueueCreate(BLUETOOTH_QUEUE_SIZE,BRS2MD_SIZE);
    g_pBluetoothReceiveQueue = xQueueCreate(BLUETOOTH_QUEUE_SIZE,MD2BRS_SIZE);

    //Seed the Random Number
    srand(xTaskGetTickCount());

    //
    // Create the Bluetooth task.
    //
    if(xTaskCreate(BluetoothTask, (signed portCHAR *)"Bluetooth", BLUETOOTHTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_BT_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "brs_c_bt_task.h"
#include "../../smart_data_types.h"
#include "../PCC/power_chair_command_constants.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "brs_c_led.h"
#include "brs_c_uart.h"
#include "semphr.h"

//*****************************************************************************
//
// The stack size for the UART task.
//
//*****************************************************************************
#define BLUETOOTHTASKSTACKSIZE        128         // Stack size in words
#define BLUETOOTH_SEND_QUEUE_SIZE     5           // Queue size in messages
#define BLUETOOTH_RECEIVE_QUEUE_SIZE  100         // Queue size in messages

#define BLUETOOTH_BUFFER_SIZE 50 //bytes

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
    uint8_t          buff[BLUETOOTH_BUFFER_SIZE];
    BluetoothFrame_t receivedBTFrame;
    TM_Frame_t       tmFrameToSend;
    uint32_t         bytesReceived = 0;
    int              i             = 0;
    volatile uint8_t temp = 0x00;

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
    memset(&receivedBTFrame, 0, sizeof(BluetoothFrame_t));

    // Loop forever.
    while(1)
    {
    	#ifdef BRS_DEBUG //Generate Debug Data

    	//Get a Random Bluetooth Frame
    	receivedBTFrame.remoteCommand = pcc_cmds[rand() % pcc_cmds_SIZE];
    	BlinkLED(TIVA_BLUE_LED, 1);

    	//Send The Frame to the Data Bridge Task
		xQueueSend(g_pBluetoothReceiveQueue, &receivedBTFrame, portMAX_DELAY);

		//Get the TM Frame from the UART Task and Send it
		xQueueReceive(g_pBluetoothSendQueue, &tmFrameToSend, 0);

		//Still Send the frame whether the Queue Receive was Successful or not
		SendTMFrame(&tmFrameToSend);

		#ifdef VERBOSE
		UARTprintf("Bluetooth Received \"%c\"\r\n", receivedBTFrame.remoteCommand);
		#endif

		#else  //Actually Get Data


    	//Read Whatever Data is in the Bluetooth Buffer
		if (BluetoothFrameAvailable())
		{
			//Get one Character
			UARTReceive(BT_UART, &temp, 1);

			//Status Good
			BlinkLED(TIVA_BLUE_LED, 1);

			if (temp == 'T')
			{
    			//Get one more Character
				if (ROM_UARTCharsAvail(BT_UART))
				{
	    			UARTReceive(BT_UART, &temp, 1);

	    			//Status Good
					BlinkLED(TIVA_BLUE_LED, 1);
				}

    			// "TM" indicates a Request for Telemetry, So Send the Frame
    			if (temp == 'M')
    			{
					#ifdef VERBOSE
    				UARTprintf("Bluetooth Received \"TM\"\r\n", temp);
					#endif

           			//Get the TM Frame from the UART Task and Send it
           	    	xQueueReceive(g_pBluetoothSendQueue, &tmFrameToSend, 0);

           	    	//Still Send the frame whether the Queue Receive was Successful or not
           	    	SendTMFrame(&tmFrameToSend);

					#ifdef VERBOSE
					UARTprintf("TM Frame Sent\r\n", temp);
					#endif
    			}
			}
			else //Must be a remote Command
			{
				receivedBTFrame.remoteCommand = (PCC_Command_Type) temp;

				#ifdef VERBOSE
				UARTprintf("Bluetooth Received \"%c\"\r\n", (unsigned char) temp);
				#endif

				//Send the Data to the Data Bridge
    			if(xQueueSend(g_pBluetoothReceiveQueue, &receivedBTFrame, portMAX_DELAY) != pdPASS)
    			{
    				// Error. The queue should never be full. If so print the
    				// error message on UART and wait for ever.
    				UARTprintf("\nQueue full. This should never happen.\n");
    				while(1)
    				{
    					BlinkLED(TIVA_RED_LED, 5);
    				}
    			}
			}
		}

		#endif

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
    #ifdef VERBOSE
    UARTprintf("Initializing Bluetooth Task...\n");
    #endif
    //
    // Create a queue for sending/receiving messages
    //
    g_pBluetoothSendQueue    = xQueueCreate(BLUETOOTH_SEND_QUEUE_SIZE,BRS2MD_SIZE);
    g_pBluetoothReceiveQueue = xQueueCreate(BLUETOOTH_RECEIVE_QUEUE_SIZE,MD2BRS_SIZE);

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

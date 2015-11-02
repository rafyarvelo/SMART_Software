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
#define BLUETOOTH_RECEIVE_QUEUE_SIZE  25          // Queue size in messages

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
    BluetoothFrame_t receivedBTFrame;
    TM_Frame_t       tmFrameToSend;
    uint8_t          prevByte = 0x00;
    uint8_t          currByte = 0x00;
    uint8_t          maxBytesToRead = BLUETOOTH_RECEIVE_QUEUE_SIZE;

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
    memset(&tmFrameToSend,   0, sizeof(TM_Frame_t));

    // Loop forever.
    while(1)
    {
    	#if 0 //Generate Debug Data

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
		while (ROM_UARTCharsAvail(BT_UART))
		{
			//Get one Character from the Buffer
			currByte = ROM_UARTCharGetNonBlocking(BT_UART);

			#ifdef VERBOSE
			UARTprintf("Bluetooth Received \"%c\"\r\n", currByte);
			#endif

			//Check if the Bluetooth Device is Requesting a TM Frame
			if (prevByte == 'T' && currByte == 'M')
			{
				//Get the TM Frame from the UART Task and Send it
				xQueueReceive(g_pBluetoothSendQueue, &tmFrameToSend, 0);

				//Still Send the frame whether the Queue Receive was Successful or not
				SendTMFrame(&tmFrameToSend);

				#ifdef VERBOSE
				UARTprintf("TM Frame Sent\r\n");
				#endif
			}

			else //Must Be a Remote Command
			{
				receivedBTFrame.remoteCommand = (PCC_Command_Type) currByte;

				//Send the Data to the Data Bridge
				xQueueSend(g_pBluetoothReceiveQueue, &receivedBTFrame, portMAX_DELAY);
			}
			prevByte = currByte;
		}
		#endif

    	//Delay
    	vTaskDelay(BT_TASK_DELAY);
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

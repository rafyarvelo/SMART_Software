#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "brs_c_sensor_task.h"
#include "../../smart_data_types.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//*****************************************************************************
//
// The stack size for the Sensor task.
//
//*****************************************************************************
#define SENSORTASKSTACKSIZE        128         // Stack size in words
#define SENSOR_QUEUE_SIZE          5           // Queue size in messages

//*****************************************************************************
//
// Queue Handles Needed
//
//*****************************************************************************
xQueueHandle g_pSensorDataQueue;

static void SensorTask(void *pvParameters)
{
    portTickType ui32WakeTime;
    SensorData_t* pSensorData = malloc(sizeof(SensorData_t));

    //
    // Get the current tick count.
    //
    ui32WakeTime = xTaskGetTickCount();

    //Initialize sensor data to 0
    memset(pSensorData, 0, sizeof(SensorData_t));

    //
    // Loop forever.
    //
    while(1)
    {
    	//Generate Debug Data
		#ifdef DEBUG_ONLY
    		pSensorData->gpsData.altitude              = rand();
    		pSensorData->gpsData.latitude              = rand();
    		pSensorData->gpsData.longitude             = rand();
    		pSensorData->gpsData.groundSpeed           = rand();
    		pSensorData->rangeFinderData.rangeToObject = rand();

    	//Actually Get Data
		#else

		#endif

    	//Pass to Data Bridge
        if(xQueueSend(g_pSensorDataQueue, &pSensorData, portMAX_DELAY) != pdPASS)
        {
            // Error. The queue should never be full. If so print the
            // error message on UART and wait for ever.
            UARTprintf("\nQueue full. This should never happen.\n");
            while(1)
            {
            }
        }
    	//Do Stuff
    	vTaskDelayUntil(&ui32WakeTime, SENSOR_TASK_DELAY / portTICK_RATE_MS);//Do Stuff
    }
}

//*****************************************************************************
//
// Initializes the Bluetooth task.
//
//*****************************************************************************
uint32_t SensorTaskInit(void)
{
    UARTprintf("Initializing Sensor Task...\n");

    //
    // Create a queue for sending/receiving messages
    //
    g_pSensorDataQueue = xQueueCreate(SENSOR_QUEUE_SIZE, SENSOR_DATA_SIZE);

    //Seed the Random Number
    srand(xTaskGetTickCount());

    //
    // Create the Sensor task.
    //
    if(xTaskCreate(SensorTask, (signed portCHAR *)"Sensor", SENSORTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_SENSOR_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}

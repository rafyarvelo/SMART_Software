//*****************************************************************************
//
// SMART Software: BRS Main Processing Function
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "brs_c_uart.h"
#include "../../smart_data_types.h"
#include "led_task.h"
#include "switch_task.h"
#include "brs_c_uart_task.h"
#include "brs_c_data_bridge_task.h"
#include "brs_c_sensor_task.h"
#include "brs_c_led.h"
#include "brs_c_bt_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>FreeRTOS Example (freertos_demo)</h1>
//!
//! This application demonstrates the use of FreeRTOS on Launchpad.
//!
//! The application blinks the user-selected LED at a user-selected frequency.
//! To select the LED press the left button and to select the frequency press
//! the right button.  The UART outputs the application status at 115,200 baud,
//! 8-n-1 mode.
//!
//! This application utilizes FreeRTOS to perform the tasks in a concurrent
//! fashion.  The following tasks are created:
//!
//! - An LED task, which blinks the user-selected on-board LED at a
//!   user-selected rate (changed via the buttons).
//!
//! - A Switch task, which monitors the buttons pressed and passes the
//!   information to LED task.
//!
//! In addition to the tasks, this application also uses the following FreeRTOS
//! resources:
//!
//! - A Queue to enable information transfer between tasks.
//!
//! - A Semaphore to guard the resource, UART, from access by multiple tasks at
//!   the same time.
//!
//! - A non-blocking FreeRTOS Delay to put the tasks in blocked state when they
//!   have nothing to do.
//!
//! For additional details on FreeRTOS, refer to the FreeRTOS web page at:
//! http://www.freertos.org/
//
//*****************************************************************************


//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

//Begin all the FreeRTOS Tasks
void StartTasks();

//Remove these Flags when the Tactical Configuration is Ready

//Data Buffers
uint8_t      prevByte = 0x00;
uint8_t      currByte = 0x00;
BRS_Frame_t  brsFrame;
TM_Frame_t   tmFrame;
SensorData_t sensorData;

int main(void)
{
	uint8_t byteReceived = FALSE;

    //
    // Set the clocking to run at 50 MHz from the PLL.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);

    // Initialize the UARTs and configure them it for 115,200, 8-N-1 operation.
    ConfigureUARTs();

    //Configure the LEDs to be RGB Enabled
    ConfigureLEDs();

	#ifdef ENABLE_CONSOLE
    UARTprintf("Yes... The console is Working...\r\n");
	#endif

    //Initialize Buffers
    memset(&tmFrame, 0, sizeof(TM_Frame_t));
    memset(&sensorData, 0, sizeof(SensorData_t));
    memset(&brsFrame,   0, sizeof(BRS_Frame_t));
    memcpy(&brsFrame.MsgId, BRS2BCI_MSG_ID, MSG_ID_SIZE);
    memset(&brsFrame.remoteCommand, PCC_CMD_NONE, 1);

    //Execute BRS Code Forever
    while (1)
    {
    	//Check for TM Frame
    	if (ROM_UARTCharsAvail(BCI_UART))
    	{
    		ReadBCI2BRSMsg(&tmFrame);
    	}

    	//Check for Bluetooth Characters
    	if (ROM_UARTCharsAvail(BT_UART))
    	{
        	currByte = ROM_UARTCharGetNonBlocking(BT_UART);
        	byteReceived = TRUE;

        	#ifdef ENABLE_CONSOLE
			UARTprintf("Bluetooth Received %c\r\n", currByte);
			#endif
    	}

		//Read one more byte in the case of a TM Request
    	if ('T' == currByte)
    	{
    		prevByte = currByte;
        	currByte = ROM_UARTCharGetNonBlocking(BT_UART);
    	}

    	//Send TM When Requested
    	if ('T' == prevByte && 'M' == currByte)
    	{
			#ifdef ENABLE_CONSOLE
        	UARTprintf("Sending TM Frame\r\n");
			#endif
    		SendTMFrame(&tmFrame);
    	}

    	else //Remote Command
    	{
    		brsFrame.remoteCommand = currByte;
    	}

    	//Check for Sensor Data
    	if (ROM_UARTCharsAvail(GPS_UART))
    	{
    		ReadGPSData(&sensorData.gpsData);
    	}

    	if (ROM_UARTCharsAvail(USF_UART) || ROM_UARTCharsAvail(USR_UART))
    	{
    		ReadUSData(&sensorData.rangeFinderData);
    	}

    	//Combine Sensor Data and Remote Data
    	memcpy(&brsFrame.sensorData, &sensorData, sizeof(SensorData_t));

    	//Send BRS Frame
		#ifdef ENABLE_CONSOLE
    	UARTprintf("Sending BRS Frame...\r\n");
    	#endif

    	//Reset Remote Command before send if no bluetooth frame was received
    	if (!byteReceived)
    	{
    		brsFrame.remoteCommand = PCC_CMD_NONE;
    	}
    	SendBRSFrame(&brsFrame);

    	//Reset default values
    	brsFrame.remoteCommand = PCC_CMD_NONE;
    	byteReceived = FALSE;
    	prevByte = currByte;

    	//Relax for a bit
    	SysCtlDelay(SysCtlClockGet() / 10 / 5);
    }
}

void StartTasks()
{
	// Create the UART task.
    if(UARTTaskInit() != 0)
    {
    	UARTprintf("Couldn't Start UART Task:(");
        while(1)
        {
        }
    }

    // Create the Bluetooth task.
    if(BluetoothTaskInit() != 0)
    {
    	UARTprintf("Couldn't Start BT Task:(");
        while(1)
        {
        }
    }

    // Create the Sensor task.
    if(SensorTaskInit() != 0)
    {
    	UARTprintf("Couldn't Start Sensor Task:(");
        while(1)
        {
        }
    }

    // Create the Data Bridge task.
    if(DataBridgeTaskInit() != 0)
    {
    	UARTprintf("Couldn't Start Data Bridge Task:(");
        while(1)
        {
        }
    }
}

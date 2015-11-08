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
#include "brs_c_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

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

//GPS Sentence Buffer
char GPS_NMEA_SENTENCE[GPS_NMEA_MAX_WORD_SIZE][GPS_NMEA_MAX_SENTENCE_SIZE];

int main(void)
{
	uint8_t      tmFrameReceived = FALSE;
	uint8_t      currByte = 0x00;
	BRS_Frame_t  brsFrame;
	TM_Frame_t   tmFrame;
	volatile unsigned int iterations = 1;

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
    memset(&tmFrame , 0, sizeof(TM_Frame_t));
    memset(&brsFrame, 0, sizeof(BRS_Frame_t));

    //Initialize Default Values
    memcpy(&brsFrame.MsgId, BRS2BCI_MSG_ID, MSG_ID_SIZE);
    brsFrame.remoteCommand = PCC_CMD_NONE;
    brsFrame.sensorData.rangeFinderData.rangeBack  = MAX_RANGE_TO_OBJECT;
    brsFrame.sensorData.rangeFinderData.rangeFront = MAX_RANGE_TO_OBJECT;

    //Execute BRS Code Forever
    while (1)
    {
    	//Check for TM Frame
    	if (ROM_UARTCharsAvail(BCI_UART))
    	{
    		ReadBCI2BRSMsg(&tmFrame);
    		tmFrameReceived = TRUE; //latch
    	}

		#ifdef DEBUG_ONLY
    	brsFrame.remoteCommand = "frbl"[rand() % 4];

    	#else //Actually Get the Data
    	//Check for Bluetooth Characters
    	if (ROM_UARTCharsAvail(BT_UART))
    	{
        	currByte = ROM_UARTCharGetNonBlocking(BT_UART);

        	#ifdef ENABLE_CONSOLE
			UARTprintf("Bluetooth Received %c\r\n", currByte);
			#endif

			//Update Remote Command
	   		brsFrame.remoteCommand = currByte;
    	}
    	else //Remote Command
    	{
    		brsFrame.remoteCommand = PCC_CMD_NONE;
    	}
		#endif

		#ifdef DEBUG_ONLY
    	brsFrame.sensorData.gpsData.altitude           = rand() % 100 * 3.14;
		brsFrame.sensorData.gpsData.latitude           = rand() % 100 * 3.14;
		brsFrame.sensorData.gpsData.longitude          = rand() % 100 * 3.14;
		brsFrame.sensorData.gpsData.groundSpeed        = rand() % 100 * 3.14;
		brsFrame.sensorData.rangeFinderData.rangeFront = rand() % 100 * 3.14;
		brsFrame.sensorData.rangeFinderData.rangeBack  = rand() % 100 * 3.14;

		#else //Actually Get the Data

		ReadGPSData(&brsFrame.sensorData.gpsData);
		ReadUSData(&brsFrame.sensorData.rangeFinderData);

		#endif

    	//Send Frame to BCI Processor if it is not a TM Request
    	if (currByte != 'T' && currByte != 'M')
    	{
			#ifdef ENABLE_CONSOLE
			UARTprintf("Sending %c\r\n", brsFrame.remoteCommand);
			#endif
        	SendBRSFrame(&brsFrame);
    	}

    	//Once we receive the first TM Frame start sending out a TM Stream to the Bluetooth device
    	if (tmFrameReceived)
    	{
    		//Change Message Id and send through Bluetooth Module
    		memcpy(&tmFrame.MsgId, BRS2MD_MSG_ID, MSG_ID_SIZE);
        	SendTMFrame(&tmFrame);
    	}

    	//Reset default values
    	brsFrame.remoteCommand = PCC_CMD_NONE;
    	currByte = PCC_CMD_NONE;

    	//Relax for a bit
    	//SysCtlDelay(SysCtlClockGet() / 10 / 5);
    	iterations++;
    }
}

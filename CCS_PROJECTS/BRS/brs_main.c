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

//Test Everything
#undef DEBUG_ONLY
#define ENABLE_CONSOLE
//#define FRAME_DEBUG

#ifdef FRAME_DEBUG
void GenerateRandomSensorData(SensorData_t* pFrame);
void GenerateRandomTM(TM_Frame_t* pFrame);
#endif

int main(void)
{
	uint8_t      currByte = 0x00;
	BRS_Frame_t  brsFrame;
	TM_Frame_t   tmFrame;
	uint8_t      newDataAvailable = FALSE;
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
    brsFrame.sensorData.gpsData.altitude    = GPS_DEFAULT_ALTITUDE;
    brsFrame.sensorData.gpsData.latitude    = GPS_DEFAULT_LATITUDE;
    brsFrame.sensorData.gpsData.longitude   = GPS_DEFAULT_LONGITUDE;
    brsFrame.sensorData.gpsData.groundSpeed = GPS_DEFAULT_SPEED;

    //Initialize the TM Frame
    tmFrame.timeStamp = 0;
    tmFrame.bciState = BCI_OFF;
    memcpy(&tmFrame.brsFrame, &brsFrame, sizeof(BRS_Frame_t));
    tmFrame.lastCommand = PCC_CMD_NONE;
    tmFrame.lastConfidence = UNSURE;
    tmFrame.processingResult.command = PCC_CMD_NONE;
    tmFrame.processingResult.confidence = UNSURE;
    tmFrame.ledForward.frequency  = LED_FORWARD_FREQ_DEFAULT;
    tmFrame.ledBackward.frequency = LED_BACKWARD_FREQ_DEFAULT;
    tmFrame.ledRight.frequency    = LED_RIGHT_FREQ_DEFAULT;
    tmFrame.ledLeft.frequency     = LED_LEFT_FREQ_DEFAULT;

    while (0)
    {
    	ReadGPSData(&brsFrame.sensorData.gpsData);
//    	while (ROM_UARTCharsAvail(GPS_UART))
//    	{
//    		ROM_UARTCharPut(CONSOLE_UART, ROM_UARTCharGet(GPS_UART));
//    	}
    }

    //Execute BRS Code Forever
    while (1)
    {
    	//Check for TM Frame
    	if (ROM_UARTCharsAvail(BCI_UART))
    	{
    		newDataAvailable = ReadBCI2BRSMsg(&tmFrame);
    	}

		#ifdef DEBUG_ONLY
    	brsFrame.remoteCommand = "frbl"[rand() % 4];

    	#else //Actually Get the Data

    	//Check for Bluetooth Characters
    	if (ROM_UARTCharsAvail(BT_UART))
    	{
        	currByte = ROM_UARTCharGet(BT_UART);

			//Update Remote Command
	   		brsFrame.remoteCommand = currByte;

			#ifdef ENABLE_CONSOLE
			UARTprintf("Bluetooth Received %c\r\n", brsFrame.remoteCommand);
			#endif

			newDataAvailable = TRUE;
    	}
    	else //Remote Command
    	{
    		brsFrame.remoteCommand = PCC_CMD_NONE;
    	}
		#endif

		#ifdef DEBUG_ONLY

    	//Create Random Sensor Data
    	GenerateRandomSensorData(&brsFrame.sensorData);
		newDataAvailable = TRUE;

		#else

    	//Check for US Data
		if (ReadUSData(&brsFrame.sensorData.rangeFinderData))
		{
			newDataAvailable = TRUE;
		}

		//Only Read GPS Data when TM is being requested
    	if (currByte == 'T' || currByte == 'M')
    	{
    		if (ReadGPSData(&brsFrame.sensorData.gpsData))
    		{
    			newDataAvailable = TRUE;
    		}
    	}

		#endif

		//Don't Bother Sending duplicate frames
		if (newDataAvailable)
		{
	    	//Send Frame to BCI Processor if it is not a TM Request
	    	if (currByte == 'T' || currByte == 'M')
	    	{
	    		//Change Message Id and send through Bluetooth Module
	    		memcpy(&tmFrame.MsgId, BRS2MD_MSG_ID, MSG_ID_SIZE);

	    		#ifdef FRAME_DEBUG
	    		GenerateRandomTM(&tmFrame);
	    		#endif

	        	SendTMFrame(&tmFrame);
	    	}

	    	else //Must be a remote command
	    	{
				#ifdef ENABLE_CONSOLE
				UARTprintf("Sending %c\r\n", brsFrame.remoteCommand);
				#endif

				#ifdef FRAME_DEBUG
				GenerateRandomSensorData(&brsFrame.sensorData);
				#endif

				SendBRSFrame(&brsFrame);
	    	}
		}

    	//Reset default values
    	brsFrame.remoteCommand = PCC_CMD_NONE;
    	currByte = PCC_CMD_NONE;
    	newDataAvailable = FALSE;

		#ifdef DEBUG_ONLY
    	SysCtlDelay(SysClockGet() / 5);
		#endif
    }
}

#ifdef FRAME_DEBUG
void GenerateRandomSensorData(SensorData_t* pFrame)
{
	float floats[] = { 1.2, 12.3, 121.4, 23.1 };

	pFrame->gpsData.altitude = floats[rand() % 4];
	pFrame->gpsData.latitude = floats[rand() % 4];
	pFrame->gpsData.longitude = floats[rand() % 4];
	pFrame->gpsData.groundSpeed = floats[rand() % 4];
	pFrame->rangeFinderData.rangeFront = floats[rand() % 4];
	pFrame->rangeFinderData.rangeBack = floats[rand() % 4];
}

void GenerateRandomTM(TM_Frame_t* pFrame)
{
	const char* pccCommands = "fbrl";

	pFrame->timeStamp = rand() % 5;
	pFrame->bciState  = rand() % 5;
	pFrame->lastCommand = pccCommands[rand() % 4];
	pFrame->lastConfidence = rand() % 4;
	pFrame->processingResult.command = pccCommands[rand() % 4];
	pFrame->processingResult.confidence = rand() % 4;
	pFrame->brsFrame.remoteCommand = pccCommands[rand() % 4];
	GenerateRandomSensorData(&pFrame->brsFrame.sensorData);
	pFrame->ledForward.id = LED_FORWARD;
	pFrame->ledForward.frequency = LED_FORWARD_FREQ_DEFAULT;
	pFrame->ledBackward.id = LED_BACKWARD;
	pFrame->ledBackward.frequency = LED_BACKWARD_FREQ_DEFAULT;
	pFrame->ledRight.id = LED_RIGHT;
	pFrame->ledRight.frequency = LED_RIGHT_FREQ_DEFAULT;
	pFrame->ledLeft.id = LED_LEFT;
	pFrame->ledLeft.frequency = LED_LEFT_FREQ_DEFAULT;

	pFrame->pccConnectionStatus = CONNECTED;
	pFrame->brsConnectionStatus = CONNECTED;
	pFrame->eegConnectionStatus = CONNECTED;
	pFrame->flasherConnectionStatus = CONNECTED;
}
#endif

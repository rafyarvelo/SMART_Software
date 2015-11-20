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
//#define ENABLE_CONSOLE
//#define FRAME_DEBUG

#ifdef FRAME_DEBUG
void GenerateRandomSensorData(SensorData_t* pFrame);
void GenerateRandomTM(TM_Frame_t* pFrame);
#endif

int main(void)

{
	uint8_t      currByte = 0x00;
	TM_Frame_t   tmFrame;
	uint8_t      newDataAvailable = FALSE;
	uint8_t      tmFrameRequested = FALSE;
	volatile unsigned int iterations = 1;
	BCI2BRS_MSG  bciMsg;

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
    memset(&bciMsg , 0, sizeof(BCI2BRS_MSG));

    //Initialize the TM Frame
	//Change Message Id and send through Bluetooth Module
	memcpy(&tmFrame.MsgId, BRS2MD_MSG_ID, MSG_ID_SIZE);
    tmFrame.timeStamp = 0;
    tmFrame.bciState = BCI_OFF;
    tmFrame.lastCommand = PCC_CMD_NONE;
    tmFrame.lastConfidence = UNSURE;
    tmFrame.processingResult.command = PCC_CMD_NONE;
    tmFrame.processingResult.confidence = UNSURE;
    tmFrame.ledForward.frequency  = LED_FORWARD_FREQ_DEFAULT;
    tmFrame.ledBackward.frequency = LED_BACKWARD_FREQ_DEFAULT;
    tmFrame.ledRight.frequency    = LED_RIGHT_FREQ_DEFAULT;
    tmFrame.ledLeft.frequency     = LED_LEFT_FREQ_DEFAULT;
    memcpy(&tmFrame.brsFrame.MsgId, BRS2BCI_MSG_ID, MSG_ID_SIZE);
    tmFrame.brsFrame.remoteCommand = PCC_CMD_NONE;
    tmFrame.brsFrame.sensorData.rangeFinderData.rangeBack  = MAX_RANGE_TO_OBJECT;
    tmFrame.brsFrame.sensorData.rangeFinderData.rangeFront = MAX_RANGE_TO_OBJECT;
    tmFrame.brsFrame.sensorData.gpsData.altitude    = GPS_DEFAULT_ALTITUDE;
    tmFrame.brsFrame.sensorData.gpsData.latitude    = GPS_DEFAULT_LATITUDE;
    tmFrame.brsFrame.sensorData.gpsData.longitude   = GPS_DEFAULT_LONGITUDE;
    tmFrame.brsFrame.sensorData.gpsData.groundSpeed = GPS_DEFAULT_SPEED;


    //For GPS Testing, REMOVE ME
    while (0)
    {
    	while (ROM_UARTCharsAvail(GPS_UART))
    	{
    		ROM_UARTCharPut(CONSOLE_UART, ROM_UARTCharGet(GPS_UART));
    	}
    }

    //Execute BRS Code Forever
    while (1)
    {
    	if (iterations++ % 100000 == 0)
    	{
    		newDataAvailable = TRUE;
    		tmFrame.timeStamp += 501;
    	}

    	//Check for TM Frame
    	if (ROM_UARTCharsAvail(BCI_UART))
    	{
    		UARTReceive(BCI_UART, (volatile uint8_t*) &bciMsg, sizeof(BCI2BRS_MSG));

    		switch (bciMsg.eegCmd)
    		{
				case PCC_FORWARD:
				case PCC_BACKWARD:
				case PCC_RIGHT:
				case PCC_LEFT:
				case PCC_CMD_NONE:
					tmFrame.processingResult.command = bciMsg.eegCmd;
					tmFrame.lastCommand = bciMsg.eegCmd;
				break;
    		}

    		switch (bciMsg.eegConfidence)
    		{
				case UNSURE:
				case MODERATE:
				case LIKELY:
				case ABSOLUTE:
					tmFrame.processingResult.confidence = bciMsg.eegConfidence;
					tmFrame.lastConfidence              = bciMsg.eegConfidence;
				break;
    		}

    		switch (bciMsg.bciState)
    		{
				case BCI_OFF:
				case BCI_INITIALIZATION:
				case BCI_STANDBY:
				case BCI_PROCESSING:
				case BCI_READY:
					tmFrame.bciState = bciMsg.bciState;
				break;
    		}
    	}

		#ifdef DEBUG_ONLY
    	brsFrame.remoteCommand = "frbl"[rand() % 4];

    	#else //Actually Get the Data

    	//Check for Bluetooth Characters
    	if (ROM_UARTCharsAvail(BT_UART))
    	{
        	currByte = ROM_UARTCharGet(BT_UART);

			//Update Remote Command
        	if (currByte == 'T' || currByte == 'M')
        	{
        		tmFrameRequested = TRUE;
        	}
        	else
        	{
    	   		tmFrame.brsFrame.remoteCommand = currByte;
    	   		tmFrame.lastCommand = currByte;
    	   		tmFrame.bciState    = BCI_PROCESSING;
        	}

			#ifdef ENABLE_CONSOLE
			UARTprintf("Bluetooth Received %c\r\n", tmFrame.brsFrame.remoteCommand);
			#endif

			newDataAvailable = TRUE;
    	}
    	else //Remote Command
    	{
    		tmFrame.brsFrame.remoteCommand = PCC_CMD_NONE;
    	}
		#endif

		#ifdef DEBUG_ONLY

    	//Create Random Sensor Data
    	GenerateRandomSensorData(&tmFrame.brsFrame.sensorData);
		newDataAvailable = TRUE;

		#else

    	//Check for US Data
		if (ReadUSData(&tmFrame.brsFrame.sensorData.rangeFinderData))
		{
			newDataAvailable = TRUE;
		}

		//Only Read GPS Data when TM is being requested
    	if (currByte == 'T' || currByte == 'M')
    	{
    		if (ReadGPSData(&tmFrame.brsFrame.sensorData.gpsData))
    		{
    			newDataAvailable = TRUE;
    		}
    	}

		#endif

		//Don't Bother Sending duplicate frames
		if (newDataAvailable)
		{
			switch(currByte)
			{
				case PCC_FORWARD:
				case PCC_BACKWARD:
				case PCC_RIGHT:
				case PCC_LEFT:
				case PCC_CMD_NONE:
					#ifdef ENABLE_CONSOLE
					UARTprintf("Sending %c\r\n", tmFrame.brsFrame.remoteCommand);
					#endif

					#ifdef FRAME_DEBUG
					GenerateRandomSensorData(&tmFrame.brsFrame.sensorData);
					#endif

					SendBRSFrame(&tmFrame.brsFrame);
					break;
				case 'T':
				case 'M':
					#ifdef FRAME_DEBUG
					GenerateRandomTM(&tmFrame);
					#endif

					SendTMFrame(&tmFrame);
					break;
			}
		}

    	//Reset default values
    	tmFrame.brsFrame.remoteCommand = PCC_CMD_NONE;
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

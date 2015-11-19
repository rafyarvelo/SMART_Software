/*
 * brs_c_uart.c
 *
 *  Created on: Sep 25, 2015
 *      Author: Rafael
 */

#include "brs_c_uart.h"
#include "utils/uartstdio.h"
#include <string.h>
#include <math.h>

//#define ENABLE_CONSOLE

#define MAX_TRIES 500
char GPS_Sentence[GPS_NMEA_MAX_WORD_SIZE][GSV_NUM_WORDS + 1];

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Configure the UARTs and their pins.  This must be called before UARTSend().
//
//*****************************************************************************
void ConfigureUARTs(void)
{
	//The 5 Five UARTs we will need for our application
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable UARTs
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinConfigure(GPIO_PC4_U1RX);
    ROM_GPIOPinConfigure(GPIO_PC5_U1TX);
    ROM_GPIOPinConfigure(GPIO_PD6_U2RX);
    ROM_GPIOPinConfigure(GPIO_PD7_U2TX);
    ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
    ROM_GPIOPinConfigure(GPIO_PC7_U3TX);
    ROM_GPIOPinConfigure(GPIO_PE4_U5RX);
    ROM_GPIOPinConfigure(GPIO_PE5_U5TX);
    ROM_GPIOPinConfigure(GPIO_PD4_U6RX);
    ROM_GPIOPinConfigure(GPIO_PD5_U6TX);

    //
    // Configure Pin Types
    //
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet (UART0_BASE, UART_CLOCK_PIOSC);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200    , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 115200    , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), BAUD_RATE , (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));

    //
    // Initialize the CONSOLE UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(UART_ID uartID, const uint8_t *pui8Buffer, uint32_t ui32Count)
{
	if (pui8Buffer == NULL)
	{
		return;
	}

    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPut(uartID, *pui8Buffer++);
    }
}

//*****************************************************************************
//
// Retrieve a string from the UART, return bytes actually read
//
//*****************************************************************************
uint16_t UARTReceive(UART_ID uartID, volatile uint8_t *pui8Buffer, uint32_t ui32Count)
{
	uint16_t bytesReceived = 0;

	//
    // Loop while there are characters in the receive FIFO.
    //
    while(ROM_UARTCharsAvail(uartID) && ui32Count--)
    {
        //
        // Read the next character from the UART and put it in the buffer
        //
    	*pui8Buffer++ = ROM_UARTCharGet(uartID);

        //Increment the Number of Bytes Received
    	bytesReceived++;
    }

    return bytesReceived;
}

//*****************************************************************************
//
// Retrieve a string from the UART until a delimeter is found, return bytes actually read
//
//*****************************************************************************
uint16_t UARTReceiveUntil(UART_ID uartID, char *pui8Buffer, char delim, uint32_t maxSize)
{
	uint16_t bytesReceived = 0;
	uint16_t timesTried = 0;
	uint8_t   currChar = 0xFF;

	//
    // Loop while there are characters in the receive FIFO.
    //
    while(timesTried++ < MAX_TRIES)
    {
    	if (ROM_UARTCharsAvail(uartID))
    	{
            //
            // Read the next character from the UART and put it in the buffer
            //
      		currChar = ROM_UARTCharGet(uartID);

        	//Continue until delimeter is found
        	if (currChar == delim || currChar == '\r' || currChar == '\n' || currChar == '\0')
        	{
        		break;
        	}
        	else
        	{
        		//Update the buffer
         		 if (pui8Buffer != NULL)
         		 {
         			 *pui8Buffer++ = currChar;
         		 }

        		//Increment the Number of Bytes Received
            	if (bytesReceived++ >= maxSize)
            	{
            		return bytesReceived;
            	}
        	}
    	}
    }

    return bytesReceived;
}

//*****************************************************************************
//
// Retrieve all the delimeted words until a CRLF is seen and place in a buffer.
// Return the number of words read
//
//*****************************************************************************
uint16_t UARTReadDelimetedLine(UART_ID uartID, char** pui8DoubleBuffer, char delim)
{
	uint8_t           currByte  = 0x00;
	uint16_t          wordsRead = 0;
	char* pCurrWord = NULL;

	//Read the sentence into a buffer
	while (ROM_UARTCharsAvail(uartID) && currByte != '\n')
	{
		//Get a pointer to the current word in the buffer
		pCurrWord = (char*) &pui8DoubleBuffer[wordsRead][0];

		//Get a word from the UART
		UARTReceiveUntil(uartID, pCurrWord, delim, GPS_NMEA_MAX_WORD_SIZE);

		//Keep Track of Words Actually Read
		wordsRead++;
	}

	return wordsRead;
}

//*****************************************************************************
//
// Retrieve a pointer to a substring inside a delimeted string, return NULL
// if offset is invalid or delim not found.
// * offset is the WORD INDEX of the desired delimeted word.
// * wordSize is a reference to a variable where the size of the word will be put
//*****************************************************************************
char* extractDelimetedString(char* delimetedString, uint32_t size, uint32_t offset, char delim, uint32_t* wordSize)
{
	//Check arguments
	if (delimetedString == NULL || wordSize == NULL || offset > size)
	{
		return NULL;
	}

	char*    tmp1 = delimetedString;
	char*    tmp2 = NULL;

	uint32_t wordsSeen = 0;
	uint32_t charsSeen = 0;

	//Loop until desired word is found
	while (wordsSeen < offset)
	{
		if (*tmp1 == delim)
		{
			wordsSeen++;
		}

		//Move to next Character
		tmp1++;
		charsSeen++;

		//We have reached the end of the string without finding the substr
		if (charsSeen >= size)
		{
			return NULL;
		}
	}

	//Find the Size of the extracted word
	tmp2 = tmp1;
	while (*tmp2 != delim && *tmp2 != '\0' && *tmp2 != '\n' && charsSeen < size)
	{
		tmp2++;
		charsSeen++;
		*wordSize++;
	}

	return tmp1;
}

//*****************************************************************************
//
// Convert ASCII Character to Hexadecimal
//
//*****************************************************************************
uint8_t ASCII2HEX(char asciiChar)
{
	uint8_t hexVal = 0x00;

	//0-9
	if (asciiChar >= 0x30 && asciiChar <= 0x39)
	{
		hexVal = asciiChar - 0x30;
	}
	//A-F
	else if (asciiChar >= 0x41 && asciiChar <= 0x46)
	{
		hexVal = asciiChar - 0x41;
	}
	//a-f
	else if (asciiChar >= 0x61 && asciiChar <= 0x66)
	{
		hexVal = asciiChar - 0x61;
	}

	return hexVal;
}

//*****************************************************************************
//
// Convert ASCII word to Number, read until '\0' is seen
//
//*****************************************************************************
uint32_t ASCII2UINT(const uint8_t* pui8buffer, uint32_t length)
{
	uint32_t ui32Val = 0;
	int index = 0, digit = 0;
	const uint8_t* tmp = pui8buffer;

	if (pui8buffer == NULL)
	{
		return 0;
	}

	//Find length of string
	while (length-- > 0 && *tmp++ != '\0')
	{
		index++;
	}

	//Calculate Long Value
	for (index = index - 1 ; index >= 0; index--)
	{
		ui32Val += ASCII2HEX(pui8buffer[index]) * (pow(10, digit++));
	}

	return ui32Val;
}

//*****************************************************************************
//
// Convert ASCII word to Number, read until count or '\0' is seen, return status
//
//*****************************************************************************
uint8_t ASCII2FLOAT(const uint8_t* pui8buffer, uint32_t length, float* pFloat)
{
	if (pui8buffer == NULL)
	{
		return FALSE;
	}

	int   i = 0, j = 0, len = 0, decLoc = 0;
	uint8_t decFound = FALSE;
	float fVal = 0.0;
	const uint8_t* tmp = pui8buffer;

	while (length-- > 0 && *tmp != '\0' && *tmp != '\r' && *tmp != '\n')
	{
		if (*tmp == '.')
		{
			decLoc = len;
			decFound = TRUE;
		}

		len++;
		tmp++;
	}

	//Use integer method if no decimal is found
	if (!decFound)
	{
		fVal    = (float) ASCII2UINT(pui8buffer, length);
		*pFloat = fVal;

		return TRUE;
	}

	//Get numbers to right of decimal
	for (i = len; i > decLoc; i--)
	{
		fVal += ASCII2HEX(pui8buffer[i]) * (pow(10,decLoc - i));
	}

	//Get numbers to left of decimal
	for (i = decLoc; i >= 0; i--)
	{
		fVal += ASCII2HEX(pui8buffer[i]) * (pow(10,j++));
	}

	*pFloat = fVal;
	return TRUE;
}

//*****************************************************************************
//
// Read Incoming BCI Message
//
//*****************************************************************************
uint8_t ReadBCI2BRSMsg(TM_Frame_t* pFrame)
{
	if (pFrame == NULL)
	{
		return;
	}

	//Get the Message
	UARTReceive(BCI_UART, (volatile uint8_t*) pFrame, sizeof(TM_Frame_t));

	return checkMsgID(pFrame->MsgId, BCI2BRS_MSG_ID);
}

//*****************************************************************************
//
// Read Incoming GPS Data, return TRUE if it is available
//
//*****************************************************************************
uint8_t ReadGPSData(GPS_Data_t* pData)
{
	uint8_t  i        = 0;
	uint8_t  index    = 0;
	uint8_t  numTries = 0;
	uint8_t  currChar = 0x00;
	char     gpsNMEA_ID[GPS_NMEA_ID_LEN];
	char     dataValid[GPS_NMEA_MAX_WORD_SIZE];
	char     gpsLat[GPS_NMEA_MAX_WORD_SIZE];
	char     gpsLng[GPS_NMEA_MAX_WORD_SIZE];
	char     gpsSpd[GPS_NMEA_MAX_WORD_SIZE];
	char     gpsAlt[GPS_NMEA_MAX_WORD_SIZE];

	//Initialize Buffers
	memset(&gpsNMEA_ID[0], 0, GPS_NMEA_ID_LEN);
	memset(&dataValid[0], 0, GPS_NMEA_MAX_WORD_SIZE);
	memset(&gpsLat[0], 0, GPS_NMEA_MAX_WORD_SIZE);
	memset(&gpsLng[0], 0, GPS_NMEA_MAX_WORD_SIZE);
	memset(&gpsSpd[0], 0, GPS_NMEA_MAX_WORD_SIZE);
	memset(&gpsAlt[0], 0, GPS_NMEA_MAX_WORD_SIZE);

	//Wait for the Beginning of a message ID
	while (currChar != GPS_ID_START && numTries++ < 103) //LOL
	{
		currChar = ROM_UARTCharGet(GPS_UART);
	}

	//Get the Message ID
	if (currChar == GPS_ID_START)
	{
		numTries = 0;
		index    = 0;
		gpsNMEA_ID[index++] = currChar;

		while (currChar != GPS_DATA_DELIM && numTries++ < MAX_TRIES)
		{
			if (ROM_UARTCharsAvail(GPS_UART))
			{
				currChar = ROM_UARTCharGet(GPS_UART);

				//See if we have read the entire ID
				if (currChar == GPS_DATA_DELIM)
				{

					#ifdef ENABLE_CONSOLE
					UARTprintf("GPS MSG ID = %s\r\n", gpsNMEA_ID);
					#endif

					//We only care about Recommended Min Nav Coordinates
					if (strcmp(gpsNMEA_ID, GPS_RMC_MSG_ID) == 0)
					{
						//Skip UTC Time
						//UARTReadDelimetedLine(GPS_UART, &GPS_Sentence[0][0], GPS_DATA_DELIM);
						UARTReceiveUntil(GPS_UART, NULL, GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);

						//Get Status
						UARTReceiveUntil(GPS_UART, &dataValid[0], GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);

						//Get Latitude
						UARTReceiveUntil(GPS_UART, &gpsLat[0], GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);

						//Skip N/S Indicator
						UARTReceiveUntil(GPS_UART, NULL, GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);

						//Get Longitude
						UARTReceiveUntil(GPS_UART, &gpsLng[0], GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);

						//Skip E/W Indicator
						UARTReceiveUntil(GPS_UART, NULL, GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);

						//Get Speed
						UARTReceiveUntil(GPS_UART, &gpsSpd[0], GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);

						if (strcmp(dataValid,GPS_RMC_DATA_VALID) == 0)
						{
							#ifdef ENABLE_CONSOLE
							UARTprintf("Latitude  = %s \r\n", gpsLat);
							UARTprintf("Longitude = %s \r\n", gpsLng);
							UARTprintf("Speed     = %s \r\n", gpsSpd);
							#endif

							//Update GPS Values in Struct
							ASCII2FLOAT((const uint8_t*)&gpsLat[0],GPS_NMEA_MAX_WORD_SIZE, &pData->latitude);
							ASCII2FLOAT((const uint8_t*)&gpsLng[0],GPS_NMEA_MAX_WORD_SIZE, &pData->longitude);
							ASCII2FLOAT((const uint8_t*)&gpsSpd[0],GPS_NMEA_MAX_WORD_SIZE, &pData->groundSpeed);
						}
					}
					else if (strcmp(gpsNMEA_ID, GPS_GGA_MSG_ID) == 0)
					{
						//Skip All the words until the altitude
						for (i = 1; i < GGA_MSL_ALT; i++)
						{
							UARTReceiveUntil(GPS_UART, NULL, GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);
						}

						//Get Altitude
						UARTReceiveUntil(GPS_UART, &gpsAlt[0], GPS_DATA_DELIM, GPS_NMEA_MAX_WORD_SIZE);
						ASCII2FLOAT((const uint8_t*) &gpsAlt[0],GPS_NMEA_MAX_WORD_SIZE,&pData->altitude);

						#ifdef ENABLE_CONSOLE
						UARTprintf("Altitude = %s \r\n", gpsAlt);
						#endif
					}
					else
					{
						;//Msg is not Supported
					}

					break;
				}

				//Get the Next Character in the ID
				else
				{
					gpsNMEA_ID[index++] = currChar;
				}
			}
		}

	}
	else
	{
		return FALSE; //BARNACLES
	}

	return FALSE;
}

//*****************************************************************************
//
// Read Range Finder data
//
//*****************************************************************************
uint8_t ReadUSData(US_Data_t* pData)
{
	uint8_t dataValid = FALSE;

	if (pData == NULL)
	{
		return FALSE;
	}

	//Buffers for UART data
	char c = 0x00;
	char usfData[US_UART_MSG_SIZE];
	char usrData[US_UART_MSG_SIZE];
	memset(&usrData[0], 0, US_UART_MSG_SIZE);
	memset(&usfData[0], 0, US_UART_MSG_SIZE);

	//Get Front Range Finder Data
	if (ROM_UARTCharsAvail(USF_UART))
	{
		c = ROM_UARTCharGet(USF_UART);

		//Get the Range from the UART as an ASCII Value of 000 - 255
		if (c == US_UART_DATA_START)
		{
			usfData[0] = ROM_UARTCharGet(USF_UART);
			usfData[1] = ROM_UARTCharGet(USF_UART);
			usfData[2] = ROM_UARTCharGet(USF_UART);

			#ifdef ENABLE_CONSOLE
			UARTprintf("Front Range Detected: %s Inches\r\n", usfData);
			#endif

			//Update Range Finder Data if there was not a read error
			if (usfData[0] != US_UART_DATA_START &&
				usfData[1] != US_UART_DATA_START &&
				usfData[2] != US_UART_DATA_START)
			{
				pData->rangeFront = (float) ASCII2UINT((const uint8_t*) &usfData[0], US_UART_MSG_SIZE) * INCHES2METERS;
				dataValid = TRUE;
			}
		}
	}

	//Get Rear Range Finder Data
	if (ROM_UARTCharsAvail(USR_UART))
	{
		c = ROM_UARTCharGet(USR_UART);

		//Get the Range from the UART as an ASCII Value of 000 - 255
		if (c == US_UART_DATA_START)
		{
			usrData[0] = ROM_UARTCharGet(USR_UART);
			usrData[1] = ROM_UARTCharGet(USR_UART);
			usrData[2] = ROM_UARTCharGet(USR_UART);

			#ifdef ENABLE_CONSOLE
			UARTprintf("Rear Range Detected: %s Inches\r\n", usrData);
			#endif

			//Update Range Finder Data if there was not a read error
			if (usrData[0] != US_UART_DATA_START &&
				usrData[1] != US_UART_DATA_START &&
				usrData[2] != US_UART_DATA_START)
			{
				pData->rangeBack = (float) ASCII2UINT((const uint8_t*) &usrData[0], US_UART_MSG_SIZE) * INCHES2METERS;
				dataValid = TRUE;
			}
		}
	}

	return dataValid;
}

//*****************************************************************************
//
// Send a BRS Frame through the UART
//
//*****************************************************************************
void SendBRSFrame(BRS_Frame_t* pFrame)
{
	if (pFrame == NULL)
	{
		return;
	}

	UARTSend(BCI_UART, (const uint8_t*) pFrame, sizeof(BRS_Frame_t));
}

//*****************************************************************************
//
// Send a TM Frame through the Bluetooth Module
//
//*****************************************************************************
void SendTMFrame(TM_Frame_t* pFrame)
{
	volatile unsigned int i = 0;
	const unsigned int size = sizeof(TM_Frame_t);
	const uint8_t* pData = (const uint8_t*) pFrame;
	if (pFrame == NULL)
	{
		return;
	}

	for (i = 0; i < size; i++)
	{
		//Write on byte to the Bluetoooth UART
		ROM_UARTCharPutNonBlocking(BT_UART, pData[i]);

		//Slow down every 10 frames to give receiver time to process
		if (i % 10 == 0)
		{
			SysCtlDelay(SysCtlClockGet() / 1000);
		}
	}
}

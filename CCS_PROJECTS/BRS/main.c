//*****************************************************************************
// SMART Software - BRS Main Processing
// 
//
//*****************************************************************************

#include "brs_c_uart.h"
#include "brs_c_sensor_data.h"
#include "string.h"

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int main(void)
{
	const char* msg = "Listening...\n";

	//Initialize the UART connection
	Init_UART();

	//Send Begining message
    UARTSend((uint8_t *) msg, strlen(msg));

    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {
    }
}

/*
 * brs_c_led.c
 *
 *  Created on: Sep 29, 2015
 *      Author: Rafael
 */

#include "brs_c_led.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// LED Color Access
//
// [G, R, B] range is 0 to 0xFFFF per color.
//
static uint32_t g_pui32Colors[NUM_LED_COLORS] = { 0x0000, 0x0000, 0x0000 };

//*****************************************************************************
//
// Configure the LEDs for Full RGB Functionality
//
//*****************************************************************************
void ConfigureLEDs()
{
    //
    // Initialize the GPIOs and Timers that drive the three LEDs.
    //
    RGBInit(1);
    RGBIntensitySet(0.99f);

    //Set LEDs OFF
    SetLEDRGB(0x0000, 0x0000, 0x0000);

    //Status Good
    BlinkLED(TIVA_GREEN_LED, 3);
}

//*****************************************************************************
//
// Blink an LED
//
//*****************************************************************************
void BlinkLED(LEDColor ledColor, uint8_t numTimes)
{
    uint8_t  i = 0;

    for (i = 0; i < numTimes + 1; i++)
    {
		//Turn On the LED
    	SetLED(ledColor, LED_ON);

        //
        // Delay for a bit.
        //
        SysCtlDelay(SysCtlClockGet() / 10 / 4);

		//Turn Off the LED
		SetLED(ledColor, LED_OFF);

        //
        // Delay for a bit.
        //
        SysCtlDelay(SysCtlClockGet() / 10 / 4);
    }

}

//*****************************************************************************
//
// Set an LED On or Off
//
//*****************************************************************************
void SetLED(LEDColor color, int state)
{
	if (color >= NUM_LED_COLORS)
	{
		return;
	}

	//Initialize the LEDS to OFF
	memset(g_pui32Colors, 0, 12);

	//Update the Desired LED
	g_pui32Colors[color] = state;

	//Enable RGB Colors
	RGBColorSet(g_pui32Colors);
	RGBEnable();
}

//*****************************************************************************
//
// Set LED with full RGB Functionality
//
//*****************************************************************************
void SetLEDRGB(uint32_t red, uint32_t green, uint32_t blue)
{
	g_pui32Colors[TIVA_RED_LED]   = red;
	g_pui32Colors[TIVA_GREEN_LED] = green;
	g_pui32Colors[TIVA_BLUE_LED]  = blue;

	RGBColorSet(g_pui32Colors);
	RGBEnable();
}

/*
 * pcc_gpio.h
 *
 *  Created on: Jul 5, 2015
 *      Author: Rafy
 */

#ifndef PCC_GPIO_H_
#define PCC_GPIO_H_

#include <stdio.h>
#include <msp430g2553.h>
#include "power_chair_command_constants.h"

void Init_GPIO();

//Sends Generaral input/output
void sendGPIO(volatile unsigned char* portRef, unsigned char bit, unsigned short value);

//sends a pulse for a specified duration (in ms)
void pulseOut(volatile unsigned char* portRef, unsigned char bit, unsigned short duration);

//LED FUNCTIONS
void setLED(unsigned char LED, unsigned char value);
void setLEDs(unsigned char value);

void toggleGreen();
void toggleRed();
void toggleAll();
void blinkLEDs(unsigned int numTimes);

#endif /* PCC_GPIO_H_ */

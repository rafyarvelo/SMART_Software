/*
 * pcc_cmd_control.h
 *
 *  Created on: Jul 5, 2015
 *      Author: Rafy
 */

#ifndef PCC_CMD_CONTROL_H_
#define PCC_CMD_CONTROL_H_
#pragma once

#include <stdio.h>
#include "power_chair_command_constants.h"
#include "pcc_uart_comm.h"
#include "pcc_gpio.h"

typedef enum
{
	CLOCKWISE=0,
	COUNTER_CLOCKWISE
} ServoDirection;

//This is the only function that should output Power Chair commands
void sendServoCmd(unsigned char servo, ServoDirection direction);

//Encapsulation of the sendCmd function
void Forward();
void Backward();
void Right();
void Left();
void Stop();

//Keep sending the last successfully processed command until the retry count is reached
void SendLastCommand();

//initialization
void initCmdControl();

//The Main Processing function of the PCC
void processCommand(unsigned char cmd);

//Prints the number of invalid commands seen so far
void printInvalidCmdCount(void);

void sleep(unsigned short time);

#endif /* PCC_CMD_CONTROL_H_ */

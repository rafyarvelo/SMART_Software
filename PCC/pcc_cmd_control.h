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
	COUNTER_CLOCKWISE,
	CENTER
} ServoDirection;

typedef unsigned int ServoAngle;

//This holds the values for the Servos to Point to, use it to move the servos
unsigned int servo_lut[ SERVO_STEPS+1 ];

//These are the only functions that should output Power Chair commands
void sendServoCmd(unsigned char servo, ServoDirection direction);
void rotateServo(unsigned char servo, ServoAngle angle);

//Encapsulation of the sendCmd function
void Forward     (PCC_Command_Type lastCmd);
void Backward    (PCC_Command_Type lastCmd);
void Right       (PCC_Command_Type lastCmd);
void Left        (PCC_Command_Type lastCmd);
void Stop        (PCC_Command_Type lastCmd);
void CenterServos(PCC_Command_Type lastCmd);

//initialization
void initCmdControl();
void configureServos();

//The Main Processing function of the PCC
PCC_Command_Type processCommand(PCC_Command_Type nextCmd, PCC_Command_Type lastCmd);

#endif /* PCC_CMD_CONTROL_H_ */

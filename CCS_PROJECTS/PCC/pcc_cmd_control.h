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
	SERVO_FORWARD=0,
	SERVO_BACKWARD,
	SERVO_CENTER
} ServoDirection;

//Servo LOOK UP TABLE that holds the values for the Servos to Point to, use it to move the servos
unsigned int servo_lut[ SERVO_STEPS+1 ];

//Servo Angles
typedef unsigned int ServoAngle;
#define ANGLE_FORWARD  35  //-55 Degrees in the Look up Table
#define ANGLE_CENTER   90  // 0  Degrees in the Look up Table
#define ANGLE_BACKWARD 145 //+55 Degrees in the Look up Table

//This Encapsulates the Servo Commands to only Move in 3 Directions (Forward, Back, Center)
void sendServoCmd(unsigned char servo, ServoDirection direction);

//This Physically Rotates the Servo
void rotateServo(unsigned char servo, ServoAngle angle);

//Encapsulation of the sendServoCmd function
void Forward     ();
void Backward    ();
void Right       ();
void Left        ();
void Stop        ();
void CenterServos();

//For Debugging...
void ServoTest();

//Disable PWM signal to save power
void DisablePWM();

//initialization
void initCmdControl();
void configureServos();

//The Main Processing function of the PCC
PCC_Command_Type processCommand(PCC_Command_Type nextCmd, PCC_Command_Type lastCmd);

#endif /* PCC_CMD_CONTROL_H_ */

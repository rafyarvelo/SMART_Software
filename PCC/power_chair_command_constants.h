/*
 * power_chair_command_constants.h
 *
 *  Created on: Jul 5, 2015
 *      Author: Rafy
 */

//Define all PCC Movement Commands Here
//Movement commands will be sent as UNSIGNED CHARs

#ifndef POWER_CHAIR_COMMAND_CONSTANTS_H_
#define POWER_CHAIR_COMMAND_CONSTANTS_H_

#define TRUE         1
#define FALSE        0

#define PCC_STOP     0x73 //ASCII 's'

#define PCC_FORWARD  0x66 //ASCII 'f'

#define PCC_RIGHT    0x72 //ASCII 'r'

#define PCC_LEFT     0x6C //ASCII 'l'

#define PCC_BACKWARD 0x62 //ASCII 'b'

#define PCC_CMD_NONE NULL

//===================OTHER DEFINITIONS================
//The Pins the Servos are hooked up to Port 1
#define SERVO_Y      BIT4 //Forward/Back
#define SERVO_X      BIT5 //Left/Right

#define CMD_DURATION 50 //ms
#define PULSE_DURATION 10 //ms

//LEDS
#define RED_LED      BIT0
#define GREEN_LED    BIT6

//Logic PlaceHolders
#define LOGIC_HIGH   0x01
#define LOGIC_LOW    0x00
#define LOGIC_TOGGLE 0xFF //for LED Usage

#define ON           LOGIC_HIGH
#define OFF          LOGIC_LOW

#define PORT1_REF    &P1OUT
#define PORT2_REF    &P2OUT

#define ALL_BITS_MASK 0xFF
//=====================================================

#endif /* POWER_CHAIR_COMMAND_CONSTANTS_H_ */

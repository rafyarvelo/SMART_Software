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

#include "../smart_config.h"

#define PCC_STOP     0x73 //ASCII 's'

#define PCC_FORWARD  0x66 //ASCII 'f'

#define PCC_RIGHT    0x72 //ASCII 'r'

#define PCC_LEFT     0x6C //ASCII 'l'

#define PCC_BACKWARD 0x62 //ASCII 'b'

#define PCC_TEST     0x64 //ASCII 'd' , For Testing/Debugging UART Connection

#define PCC_CMD_NONE NULL


//===================SERVO CONFIGURATION================
//Servo Output Type
#define SERVO_OUTPUT_PWM
#define SERVO_X       BIT5 //Left/Right   (Port 1 Bit 5)
#define SERVO_Y       BIT6 //Forward/Back (Port 1 Bit 6)

#define MCU_CLOCK	  1000000
#define PWM_FREQUENCY 50		// In Hertz, ideally 50Hz.

#define PWM_PERIOD    (MCU_CLOCK / PWM_FREQUENCY)	// PWM Period
#define PWM_OFF	      0							// PWM Duty Cycle 0%

// SEROVS, Connect the servo PWM SIGNAL wire to P1.6 through a 1K resistor.
#define SERVO_STEPS	180		// Maximum amount of steps in degrees (180 is common)

//Servo Specific Min and Max Pulse Widths
#ifdef TOWER_PRO_SERVO
	#define SERVO_MIN			400				// The minimum duty cycle (~.4ms,  2% )
	#define SERVO_MAX			2400			// The maximum duty cycle (~2.4ms, 12%)
#else //SERVO_TYPE_PARALLAX
	#define SERVO_MIN			PWM_PERIOD / 20	// The minimum duty cycle (~1ms, 5% )
	#define SERVO_MAX			PWM_PERIOD / 10	// The maximum duty cycle (~2ms, 10%)
#endif

//Timer A Configurations for SMCLK (We will use UP mode)
#define TIMERA_SMCLK_STOP    TASSEL_2 + MC_0
#define TIMERA_SMCLK_UP      TASSEL_2 + MC_1 //Count up to CCR0 Value, then reset
#define TIMERA_SMCLK_CONT_UP TASSEL_2 + MC_2 //Count up to 0xFFFF, then reset
#define TIMERA_SMCLK_UP_DOWN TASSEL_2 + MC_3 //Count up to CCR0 Value, then down to 0x0000, then reset

//Timer Count Modes
#define PWM_SET              OUTMOD_1 // Timer high at CCR1 Value
#define PWM_SET_RESET        OUTMOD_3 // Timer Set at CCR1, Reset at CCR0
#define PWM_RESET_SET        OUTMOD_7 // Timer Set at CCR0, Reset at CCR1

//======================================================

//LEDS
#define RED_LED      BIT0
#define GREEN_LED    BIT6

//Logic PlaceHolders
#define LOGIC_HIGH   0x01
#define LOGIC_LOW    0x00
#define LOGIC_TOGGLE 0xFF //for LED Usage

#define ON           LOGIC_HIGH
#define OFF          LOGIC_LOW

#define TRUE         1
#define FALSE        0

#define PORT1_REF    &P1OUT
#define PORT2_REF    &P2OUT

#define ALL_BITS_MASK 0xFF
//=====================================================



#endif /* POWER_CHAIR_COMMAND_CONSTANTS_H_ */

/*
 * smart_config.h
 *
 *  Created on: Jul 5, 2015
 *      Author: Rafy
 */

//Define Project Configuration here

#ifndef SMART_CONFIG_H_
#define SMART_CONFIG_H_

typedef bool ConnectionStatusType;
typedef unsigned char PCC_Command_Type;

//EEG Type (only select one)
#define EEG_DEBUG
//#define EEG_EMOTIV
//#define EEG_NAUTILUS

//Device Type (only select one)
#define ANDROID
//#define IOS
//#define WINDOWS_PHONE

//Servo Type
#define SERVO_OUTPUT_PWM

//PCC Serial Port
#ifdef WIN32
	#define PCC_PORT "COM5"
#else
	#define PCC_PORT "/dev/ttyACMO"
#endif


#endif /* SMART_CONFIG_H_ */

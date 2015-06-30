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

typedef enum
{
	EEG_TYPE_EMOTIV=0,
	EEG_TYPE_NAUTILUS,
	EEG_TYPE_DEBUG
} eegTypeEnum;

typedef enum
{
	ANDROID=0,
	IOS=1,
	WINDOWS_PHONE=2
} MobileDeviceTypeEnum;

//EEG Type (only select one)
#define EEG_DEVICE_TYPE EEG_TYPE_DEBUG

//Device Type (only select one)
#define MOBILE_DEVICE_TYPE ANDROID
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

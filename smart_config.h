/*
 * smart_config.h
 *
 *  Created on: Jul 5, 2015
 *      Author: Rafy
 */

//Define Project Configuration here

#ifndef SMART_CONFIG_H_
#define SMART_CONFIG_H_

//Basic Common Declarations
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
	#include <iostream>
	using namespace std;

	typedef bool ConnectionStatusType;
#endif

//Common Typedefs
typedef unsigned char      PCC_Command_Type;
typedef unsigned char      byteType;
typedef unsigned short     shortType;
typedef unsigned int       wordType;
typedef unsigned long      sizeType;
typedef unsigned long long longType;

#define CONNECTED     true
#define NOT_CONNECTED false

//=====================EEG Declaration======================
typedef enum
{
	EEG_TYPE_EMOTIV=0,
	EEG_TYPE_NAUTILUS,
	EEG_TYPE_DEBUG
} eegTypeEnum;

//EEG Type (only select one)
#define DEFAULT_EEG_TYPE EEG_TYPE_DEBUG //Supported Options:
                                        //EEG_TYPE_DEBUG
                                        //EEG_TYPE_NAUTILUS
                                        //EEG_TYPE_EMOTIV
//==========================================================

//==================Mobile Device Declaration================
typedef enum
{
	ANDROID=0,
	IOS,
	WINDOWS_PHONE
} MobileDeviceTypeEnum;

//Mobile Device Type (only select one)
#define DEFAULT_MOBILE_DEVICE ANDROID
                            //IOS
                            //WINDOWS_PHONE
//============================================================

//==================Servo Type Declaration====================
typedef enum
{
	PARALLAX_SERVO=0,
	TOWER_PRO_SERVO
} ServoTypeEnum;

//Servo Type
#define TOWER_PRO_SERVO

//============================================================

//PCC Serial Port
#ifdef WIN32
	#define PCC_PORT "COM5"
#else
	#define PCC_PORT "/dev/ttyACMO"
#endif

#endif /* SMART_CONFIG_H_ */

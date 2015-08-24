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

#ifdef _cplusplus
	#include <iostream>
	using namespace std;

	typedef bool               ConnectionStatusType;
#endif

typedef unsigned char      PCC_Command_Type;
typedef unsigned char      byteType;
typedef unsigned short     shortType;
typedef unsigned int       wordType;
typedef unsigned long      sizeType;
typedef unsigned long long longType;

#define CONNECTED     true
#define NOT_CONNECTED false

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
#define DEFAULT_EEG_TYPE EEG_TYPE_DEBUG //Supported Options:
                                        //EEG_TYPE_DEBUG
                                        //EEG_TYPE_NAUTILUS
                                        //EEG_TYPE_EMOTIV
//Device Type (only select one)
#define DEFAULT_MOBILE_DEVICE ANDROID
                            //IOS
                            //WINDOWS_PHONE

//PCC Serial Port
#ifdef WIN32
	#define PCC_PORT "COM5"
#else
	#define PCC_PORT "/dev/ttyACMO"
#endif

#endif /* SMART_CONFIG_H_ */

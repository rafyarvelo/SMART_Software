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
typedef u_int8_t  PCC_Command_Type;
typedef u_int8_t  byteType;
typedef u_int16_t shortType;
typedef u_int32_t wordType;
typedef u_int32_t sizeType;
typedef u_int64_t longType;

#define CONNECTED     1
#define NOT_CONNECTED 0

//=====================EEG/BRS IO Declaration======================
typedef enum
{
	EEG_TYPE_EMOTIV=0,
	EEG_TYPE_NAUTILUS,
	EEG_TYPE_DEBUG
} eegTypeEnum;

typedef enum
{
    BRS_TYPE_TIVA=0,
    BRS_TYPE_DEBUG
} brsTypeEnum;

//EEG Type (only select one)
//Supported Options:
//EEG_TYPE_DEBUG
//EEG_TYPE_NAUTILUS
//EEG_TYPE_EMOTIV


//Use Windows for Debugging by default
#if 1 //def WIN32
    #define DEFAULT_EEG_TYPE EEG_TYPE_DEBUG
    #define DEFAULT_BRS_TYPE BRS_TYPE_DEBUG
#else
    #define DEFAULT_EEG_TYPE EEG_TYPE_EMOTIV
    #define DEFAULT_BRS_TYPE BRS_TYPE_TIVA
#endif

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
    #define BRS_PORT "COM7"
#else
    #define PCC_PORT "/dev/ttyACM0"
    #define BRS_PORT "/dev/ttyAMA0"
#endif

#endif /* SMART_CONFIG_H_ */

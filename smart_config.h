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
#include <string.h>

#ifdef __cplusplus
	#include <iostream>
	using namespace std;

	typedef bool ConnectionStatusType;
#else
	typedef int  ConnectionStatusType;
#endif

//Common Typedefs
typedef unsigned char      PCC_Command_Type;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;

typedef unsigned long long uint64t;
typedef unsigned int       sizeType;

#define TRUE          1
#define FALSE         0
#define CONNECTED     TRUE
#define NOT_CONNECTED FALSE

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

typedef enum
{
    PCC_TYPE_MSP=0,
    PCC_TYPE_DEBUG
} pccTypeEnum;

//EEG Type (only select one)
//Supported Options:
//EEG_TYPE_DEBUG
//EEG_TYPE_NAUTILUS
//EEG_TYPE_EMOTIV

//Use this definition to decide which kind of configuration to run
//#define TACTICAL
#define DEBUG_ONLY

//The Actual Configuration for a real setup
#ifdef TACTICAL
    #define USE_EEG_DATA //Start the EEG Thread
    #define ENABLE_EMERGENCY_STOP //Allow E-Stop signal from Range Finder Data
    #define DEFAULT_EEG_TYPE EEG_TYPE_DEBUG//EEG_TYPE_EMOTIV
    #define DEFAULT_BRS_TYPE BRS_TYPE_TIVA
    #define DEFAULT_PCC_TYPE PCC_TYPE_MSP

//Debug Configuration for testing
#else
    #define VERBOSE //Allow Prints to Console
    #define DEFAULT_EEG_TYPE EEG_TYPE_DEBUG
    #define DEFAULT_BRS_TYPE BRS_TYPE_TIVA   //BRS_TYPE_TIVA
    #define BRS_DEBUG                        //For BRS Code on CCS
    #define DEFAULT_PCC_TYPE PCC_TYPE_MSP  //PCC_TYPE_DEBUG
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

//Serial Port Names
#define PORTNAMES_FILE "port_names.txt"

#ifdef WIN32
    #define ENDL "/r/n"
    #define WINDOWS
#else
    #define ENDL "/n"
    #define UNIX
    #define EMOTIV //Emotiv Only Compiles on Unix Machines
#endif

#endif /* SMART_CONFIG_H_ */

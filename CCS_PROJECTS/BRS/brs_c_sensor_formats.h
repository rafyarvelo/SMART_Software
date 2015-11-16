/*
 * brs_c_sensor_formats.h
 *
 *  Created on: Nov 6, 2015
 *      Author: Rafael
 */

#ifndef BRS_C_SENSOR_FORMATS_H_
#define BRS_C_SENSOR_FORMATS_H_

#include "../../smart_config.h"

//*****************************************************************************
//
// GPS Sensor Formats
//
//*****************************************************************************
#define GPS_DATA_DELIM ','
#define GPS_DATA_EOL   '\n'
#define GPS_ID_START   '$'

//Default Location is UCF SU
#define GPS_DEFAULT_LATITUDE  28.6016
#define GPS_DEFAULT_LONGITUDE 81.2005
#define GPS_DEFAULT_ALTITUDE  2.4
#define GPS_DEFAULT_SPEED     0.04

//National Marine Electronics Association (NMEA) Output Sentences
#define GPS_NMEA_MAX_CHARS 100 //100 Characters per one sentence

//NMEA Sentence Formats
#define GPS_NMEA_ID_LEN 7 //6 Characters and a NULL Terminator

//Time, position and fix type data.
#define GPS_GGA_MSG_ID "$GPGGA"
typedef enum GPSGGAMessageIndex
{
	GGA_MSG_ID=0,
	GGA_UTC_TIME,
	GGA_LATITUDE,
	GGA_NS_INDICATOR,
	GGA_LONGITUDE,
	GGA_EW_INDICATOR,
	GGA_POS_INDICATOR,
	GGA_SATS_USED,
	GGA_HDOP,
	GGA_MSL_ALT,
	GGA_MSL_ALT_UNITS,
	GGA_GEO_SEP,
	GGA_GEO_SEP_UNITS,
	GGA_ADC,
	GGA_CHKSUM,
	GGA_NUM_WORDS
} GPSGGAMessageIndex;

#define GPS_GSA_MSG_ID "$GPGSA" //GPS receiver operating mode, active satellites used in the
                                //position solution and DOP values.
typedef enum GPSGSAMessageIndex
{
	GSA_MSG_ID=0,
	GSA_MODE1,
	GSA_MODE2,
	GSA_CH1_SAT_USED,
	GSA_CH2_SAT_USED,
	GSA_CH3_SAT_USED,
	GSA_CH4_SAT_USED,
	GSA_CH5_SAT_USED,
	GSA_CH6_SAT_USED,
	GSA_CH7_SAT_USED,
	GSA_CH8_SAT_USED,
	GSA_CH9_SAT_USED,
	GSA_CH10_SAT_USED,
	GSA_CH11_SAT_USED,
	GSA_CH12_SAT_USED,
	GSA_PDOP, //Position Dilution of Precision
	GSA_HDOP, //Horizontal Dilution of Precision
	GSA_VDOP, //Vertical Dilution of Precision
	GSA_CHKSUM,
	GSA_NUM_WORDS
} GPSGSAMessageIndex;

#define GPS_GSV_MSG_ID "$GPGSV" //The number of GPS satellites in view satellite ID numbers,
                                //elevation, azimuth, and SNR values.
typedef enum GPSGSVMessageIndex
{
	GSV_MSG_ID=0,
	GSV_NUM_MSGS,
	GSV_MSG_NUM,
	GSV_SATS_IN_VIEW,
	GSV_CH1_SAT_ID,
	GSV_CH1_ELEV, //degrees
	GSV_CH1_AZIM, //degrees
	GSV_CH1_SNR,
	GSV_CH2_SAT_ID,
	GSV_CH2_ELEV, //degrees
	GSV_CH2_AZIM, //degrees
	GSV_CH2_SNR,
	GSV_CH3_SAT_ID,
	GSV_CH3_ELEV, //degrees
	GSV_CH3_AZIM, //degrees
	GSV_CH3_SNR,
	GSV_CH4_SAT_ID,
	GSV_CH4_ELEV, //degrees
	GSV_CH4_AZIM, //degrees
	GSV_CH4_SNR,
	GSV_CHKSUM,
	GSV_NUM_WORDS
} GPSGSVMessageIndex;

#define GPS_RMC_MSG_ID "$GPRMC" //Recommended Minimum Navigation (RMC).Time, date, position, course and speed data.
typedef enum GPSRMCMessageIndex
{
	RMC_MSG_ID=0,
	RMC_UTC_TIME,
	RMC_STATUS, //"A" = Valid, "V" = Invalid
	RMC_LATITUDE,
	RMC_NS_INDICATOR,
	RMC_LONGITUDE,
	RMC_EW_INDICATOR,
	RMC_SPEED,    //knots
	RMC_COURSE,   //degrees
	RMC_DATE,
	RMC_MAG_VAR,  //Magnetic Variation
	RMC_MODE,     //A = "Autonomous", D = "Differential", E = "Estimated"
	RMC_CHKSUM,
	RMC_NUM_WORDS
} GPSRMCMessageIndex;

#define GPS_RMC_DATA_VALID   "A"
#define GPS_RMC_DATA_INVALID "V"

#define GPS_VTG_MSG_ID "$GPVTG" //Course and speed information relative to the ground.
typedef enum GPSVTGMessageIndex
{
	VTG_MSG_ID=0,
	VTG_COURSE1,
	VTG_REF1, //"A" = Valid, "V" = Invalid
	VTG_COURSE2,
	VTG_REF2,
	VTG_SPEED1,
	VTG_SPEED1_UNITS, //Usually N = Knots
	VTG_SPEED2,
	VTG_SPEED2_UNITS, //Usually K = km/h
	VTG_MODE,
	VTG_CHKSUM,
	VTG_NUM_WORDS
} GPSVTGMessageIndex;

#define GPS_MODE_AUTONOMOUS   "A"
#define GPS_MODE_DIFFERENTIAL "D"
#define GPS_MODE_ESTIMATED    "E"

//Convert Knots to Meters per Second
#define GPS_KNOTS2MPS 0.514444444

//*****************************************************************************


//*****************************************************************************
//
// Ultrasonic Sensor Formats
//
//*****************************************************************************
#define US_UART_MSG_SIZE 5 //FORMAT: 'R' 000-255 \n
#define US_UART_DATA_START 'R'  //'R' for Range
#define US_UART_DATA_END   0x13 //ASCII Carriage Return

//UART Ranges come out in meters
#define INCHES2METERS 0.0254

//*****************************************************************************


#endif /* BRS_C_SENSOR_FORMATS_H_ */

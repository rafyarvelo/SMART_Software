#ifndef SMART_DATA_TYPES_H
#define SMART_DATA_TYPES_H

#include "smart_config.h"
#include "smart_message_constants.h"

//Keep this File C Compatible
#ifdef __cplusplus
	extern "C" {
#endif


//##############SMART Data Types##############

//====================EEG Types====================
typedef enum Emotiv_Electrodes
{
    F3=0, FC6, P7, T8 , F7,
    F8  , T7 , P8, AF4, F4,
    AF3 , O2 , O1, FC5, NUM_EMOTIV_ELECTRODES
} Emotiv_Electrodes;

#define MAX_RANGE_TO_OBJECT 6 //Meters
#define MAX_EEG_ELECTRODES  NUM_EMOTIV_ELECTRODES //32 (Used to be 32 because of Nautilus EEG)

//This struct defines what a single frame of EEG Data looks like
typedef struct EEG_Frame_t
{
    //The type of EEG that the frame belongs to, appended by EEG IO class
    eegTypeEnum eegType;

    uint32_t counter; //Value counter

    //Data from Each electrode, defaults to 0
    uint32_t electrodeData[MAX_EEG_ELECTRODES];

    //Contact Quality from Each electrode, defaults to 0
    uint16_t contactQuality[MAX_EEG_ELECTRODES];

    //Gyro Values
    uint8_t gyroX;
    uint8_t gyroY;

    //Percentage of Full Battery Charge
    uint8_t batteryPercentage;
} EEG_Frame_t;
//===============================================

//====================BRS Types====================
typedef struct GPS_Data_t
{
    float latitude;
    float longitude;
    float altitude;
    float groundSpeed;
} GPS_Data_t;

typedef struct US_Data_t
{
    float rangeToObject;
} US_Data_t;

typedef struct SensorData_t
{
	GPS_Data_t gpsData;
	US_Data_t  rangeFinderData;
} SensorData_t;

//Mobile Device to BRS Bluetooth Frame
typedef struct BluetoothFrame_t
{
	PCC_Command_Type remoteCommand;
} BluetoothFrame_t;

// A Frame of BRS Data
typedef struct BRS_Frame_t
{
	MSG_ID_Type      MsgId; //Message Sent from BRS to BCI
	SensorData_t     sensorData;
    PCC_Command_Type remoteCommand;
} BRS_Frame_t;
//===============================================

//==================Flasher Types==================
#define	LED_FORWARD_FREQ_DEFAULT  10
#define	LED_BACKWARD_FREQ_DEFAULT 20
#define	LED_RIGHT_FREQ_DEFAULT    30
#define	LED_LEFT_FREQ_DEFAULT     40
typedef enum
{
    LED_FORWARD=0,
    LED_BACKWARD,
    LED_RIGHT,
    LED_LEFT,
    NUM_LED_GROUPS
} LED_Group_ID;

typedef struct LED_Group_t
{
	LED_Group_ID id;
	uint16_t     frequency;
} LED_Group_t;
//=================================================

//Full Telemetry Frame
typedef struct TM_Frame_t
{
    MSG_ID_Type          MsgId; //Message Sent From BCI -> BRS -> MD
    int                  timeStamp;
    EEG_Frame_t          eegFrame; //Only the Latest Frame, EEG Telemetry is managed by the C_EEG_IO class
    BRS_Frame_t          brsFrame;
    LED_Group_t          ledForward;
    LED_Group_t          ledBackward;
    LED_Group_t          ledRight;
    LED_Group_t          ledLeft;
    ConnectionStatusType eegConnectionStatus;
    ConnectionStatusType pccConnectionStatus;
    ConnectionStatusType brsConnectionStatus;
    ConnectionStatusType flasherConnectionStatus;
} TM_Frame_t;

//############################################

//*****************************************************************************
//
// The item size for the messages, pass by pointer for speed
//
//*****************************************************************************
#define BRS2BCI_SIZE     sizeof(BRS_Frame_t*)
#define BCI2BRS_SIZE     sizeof(TM_Frame_t*)
#define MD2BRS_SIZE      sizeof(BluetoothFrame_t*)
#define BRS2MD_SIZE      sizeof(TM_Frame_t*)
#define SENSOR_DATA_SIZE sizeof(SensorData_t*)

//*****************************************************************************
//
// Data Type Creation/Initialization
//
//*****************************************************************************
TM_Frame_t*       createTMFrame();
EEG_Frame_t*      createEEGFrame();
BRS_Frame_t*      createBRSFrame();
LED_Group_t*      createLEDGroup(LED_Group_ID id);
BluetoothFrame_t* createBluetoothFrame();

#ifdef _cplusplus
	} //extern "C"
#endif

#endif //SMART_DATA_TYPES_H
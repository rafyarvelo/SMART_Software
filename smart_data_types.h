#ifndef SMART_DATA_TYPES_H
#define SMART_DATA_TYPES_H

#include "smart_config.h"
#include "CCS_PROJECTS/PCC/power_chair_command_constants.h"

////Keep this File C Compatible
#ifdef __cplusplus
    extern "C" {
#endif


//##############SMART Data Types##############
#define EMERGENCY_STOP_DISTANCE   1     //Meters
#define MAX_RANGE_TO_OBJECT       6     //Meters
#define MAX_TM_FRAMES             25    //Only store the latest 25 Frames
#define COMMAND_TIMEOUT           20000 //Wait 20 seconds before killing the System
#define	LED_FORWARD_FREQ_DEFAULT  10    //Hertz
#define	LED_BACKWARD_FREQ_DEFAULT 20    //Hertz
#define	LED_RIGHT_FREQ_DEFAULT    30    //Hertz
#define	LED_LEFT_FREQ_DEFAULT     40    //Hertz

//====================EEG Types====================
typedef enum Emotiv_Electrodes
{
    F3=0, FC6, P7, T8 , F7,
    F8  , T7 , P8, AF4, F4,
    AF3 , O2 , O1, FC5, NUM_EMOTIV_ELECTRODES
} Emotiv_Electrodes;

#if DEFAULT_EEG_TYPE == EEG_TYPE_EMOTIV
    #define MAX_EEG_ELECTRODES  NUM_EMOTIV_ELECTRODES
#else
    #define MAX_EEG_ELECTRODES  32 //(Used to be 32 because of Nautilus EEG)
#endif

//*****************************************************************************
//
// Message IDs for Communication Between Hardware
//
//*****************************************************************************

/*
 * General Message Format:
 * MESSAGE ID - 5 Bytes Unsigned (4 Characters and a Null terminator (0x00) '\0')
 * ...
 * N Bytes of Data
 *
 */

//5 Byte Message ID
#define MSG_ID_SIZE 5 //Bytes
typedef struct MsgIdType { uint8_t id[MSG_ID_SIZE]; } MsgIdType;

#define MAX_BUFFER_SIZE 1024 //How many Bytes we will store at a maximum (1KB)

//BRS Frame to the BCI
#define BRS2BCI_MSG_ID "BRS!"

//BCI TM Frame Back to the BRS
#define BCI2BRS_MSG_ID "BCI!"

//BRS to Mobile Device Communication
#define BRS2MD_MSG_ID  "BLT!"

//Mobile Device to BRS Communication
#define MD2BRS_MSG_ID  "TLB!"

//*****************************************************************************
//
// Frame Types
//
//*****************************************************************************

//Use to see how sure we are that the command is correct
typedef enum Confidence_Type
{
    UNSURE=0,
    MODERATE,
    LIKELY,
    ABSOLUTE,
    NUM_CONFIDENCE_TYPES
}Confidence_Type;

//The Final Result of our EEG Processing
typedef struct ProcessingResult_t
{
    PCC_Command_Type command;
    char             pad_0[3];
    unsigned int     confidence;

#ifdef __cplusplus
    ProcessingResult_t()
    {
        command = PCC_CMD_NONE;
        confidence = UNSURE;
    }
#endif

} ProcessingResult_t;

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
    char    pad_0[3]; //To be 4 byte aligned

#ifdef __cplusplus
    EEG_Frame_t()
    {
        eegType = DEFAULT_EEG_TYPE;
        counter = 0;
        memset(&electrodeData[0] ,0, MAX_EEG_ELECTRODES * sizeof(uint32_t));
        memset(&contactQuality[0],0, MAX_EEG_ELECTRODES * sizeof(uint16_t));
        gyroX = 0;
        gyroY = 0;
    }
#endif
} EEG_Frame_t;
//===============================================

//====================BRS Types====================
typedef struct GPS_Data_t
{
    float latitude;
    float longitude;
    float altitude;
    float groundSpeed;

#ifdef __cplusplus
    GPS_Data_t()
    {
        latitude  = 0.0;
        longitude = 0.0;
        altitude  = 0.0;
        groundSpeed = 0.0;
    }
#endif
} GPS_Data_t;

typedef struct US_Data_t
{
    float rangeFront;
    float rangeBack;

#ifdef __cplusplus
    US_Data_t()
    {
        rangeFront = MAX_RANGE_TO_OBJECT;
        rangeBack  = MAX_RANGE_TO_OBJECT;
    }
#endif
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
    char             pad_0[3];

#ifdef __cplusplus
    BluetoothFrame_t()
    {
        remoteCommand = PCC_CMD_NONE;
    }
#endif
} BluetoothFrame_t;

// A Frame of BRS Data
typedef struct BRS_Frame_t
{
	MsgIdType        MsgId; //Message Sent from BRS to BCI
    char             pad_0[3];
    PCC_Command_Type remoteCommand;
    char             pad_1[3];
	SensorData_t     sensorData;
#ifdef __cplusplus
    BRS_Frame_t()
    {
        memcpy(&MsgId, BRS2BCI_MSG_ID, MSG_ID_SIZE);
        remoteCommand = PCC_CMD_NONE;
    }
#endif
} BRS_Frame_t;
//===============================================

//==================Flasher Types==================
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
	unsigned int id;
	uint16_t     frequency;
    char         pad_0[2]; //To Be 4 byte aligned
} LED_Group_t;

//=================================================

typedef enum BCIState
{
    BCI_OFF=0,
    /*
     * 1) Remain in this state until Run() is called
     * 2) When Run() is called, Move to BCI_INITIALIZATION
     */
    BCI_INITIALIZATION,
    /*
     * 1) Create Instances of RVS, JA2BRS, BRS2JA, JA2PCC, and EEG IO
     * 2) Connect to Flasher, EEG, BRSH, and PCC
     * 3) Generate RVS Frequencies
     * 4) Send RVS to Flasher
     * 5) Send TM Packet to BRSH
     * 6) Move to BCI_STANDBY
     */
    BCI_STANDBY,
    /*
     * 1) Wait for EEG Data or Remote Commands
     * 2) Upon Receipt of EEG Data or Remote Commands, move to BCI_PROCESSING
     */
    BCI_PROCESSING,
    /*
     * 1) Process the data
     * 2) Generate PCC Command
     * 3) Move to BCI_READY
     */
    BCI_READY
    /*
     * 1) Send the Command
     * 2) Revert to BCI_STANDBY
     */
}BCIState;

//Full Telemetry Frame
typedef struct TM_Frame_t
{
    MsgIdType            MsgId; //Message Sent From BCI -> BRS -> MD
    char                 pad_0[3];
    unsigned int         timeStamp;
    unsigned int         bciState;
    PCC_Command_Type     lastCommand;
    char                 pad_1[3];
    unsigned int         lastConfidence;
    ProcessingResult_t   processingResult;
    BRS_Frame_t          brsFrame;
    LED_Group_t          ledForward;
    LED_Group_t          ledBackward;
    LED_Group_t          ledRight;
    LED_Group_t          ledLeft;
    ConnectionStatusType eegConnectionStatus;
    ConnectionStatusType pccConnectionStatus;
    ConnectionStatusType brsConnectionStatus;
    ConnectionStatusType flasherConnectionStatus;

#ifdef __cplusplus
    TM_Frame_t()
    {
        memcpy(&MsgId, BCI2BRS_MSG_ID, MSG_ID_SIZE);
        timeStamp = 0;
        bciState = BCI_OFF;
        lastCommand = PCC_CMD_NONE;
        lastConfidence = UNSURE;
        eegConnectionStatus     = NOT_CONNECTED;
        pccConnectionStatus     = NOT_CONNECTED;
        brsConnectionStatus     = NOT_CONNECTED;
        flasherConnectionStatus = NOT_CONNECTED;
    }
#endif
} TM_Frame_t;

//############################################

//*****************************************************************************
//
// The item size for the messages
//
//*****************************************************************************
#define BRS2BCI_SIZE     sizeof(BRS_Frame_t)
#define BCI2BRS_SIZE     sizeof(TM_Frame_t)
#define MD2BRS_SIZE      sizeof(BluetoothFrame_t)
#define BRS2MD_SIZE      sizeof(TM_Frame_t)
#define SENSOR_DATA_SIZE sizeof(SensorData_t)

//*****************************************************************************
//
// Data Type Creation/Initialization
//
//*****************************************************************************
TM_Frame_t*         createTMFrame();
EEG_Frame_t*        createEEGFrame();
BRS_Frame_t*        createBRSFrame();
LED_Group_t*        createLEDGroup(LED_Group_ID id);
BluetoothFrame_t*   createBluetoothFrame();
ProcessingResult_t* createProcessingResult();

//Diff two Message IDs, Return TRUE if they're equal
int checkMsgID(MsgIdType id1, char* id2);

#ifdef __cplusplus
    } //extern "C"
#endif

#endif //SMART_DATA_TYPES_H

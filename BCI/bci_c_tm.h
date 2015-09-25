#ifndef BCI_C_TM
#define BCI_C_TM

#include <QList>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "../CCS_PROJECTS/PCC/power_chair_command_constants.h"
#include "../CCS_PROJECTS/BRS/brs_c_sensor_data.h"
#include "bci_c_eeg_data.h"
#include "bci_c_rvs.h"

#define EMERGENCY_STOP_DISTANCE 1 //meters
#define MAX_TM_FRAMES           25//Only store the latest 25 Frames
//Data from the BRSH
typedef struct BRS_Frame_t
{
    GPS_Data_t    \
    gpsData;
    US_Data_t        usData;
    PCC_Command_Type remoteCommand;

    BRS_Frame_t(GPS_Data_t& gpsData, US_Data_t& usData, PCC_Command_Type cmd)
    {
        this->gpsData       = gpsData;
        this->usData        = usData;
        this->remoteCommand = cmd;
    }

    BRS_Frame_t()
    {
        memset(&gpsData,0 , sizeof(GPS_Data_t));
        memset(&usData ,0 , sizeof(US_Data_t));
        remoteCommand = PCC_CMD_NONE;
    }

    static BRS_Frame_t* create()
    {
        return new BRS_Frame_t();
    }

    static BRS_Frame_t* create(GPS_Data_t& gpsData, US_Data_t& usData, PCC_Command_Type cmd)
    {
        return new BRS_Frame_t(gpsData, usData, cmd);
    }

    BRS_Frame_t& operator =(BRS_Frame_t* rhs)
    {
        memcpy(this, rhs, sizeof(BRS_Frame_t));
        return *this;
    }

    BRS_Frame_t& operator =(BRS_Frame_t& rhs)
    {
        return operator =(&rhs);
    }


} BRS_Frame_t;

//A Single Frame of Telemetry Data
typedef struct TM_Frame_t
{
    int           timeStamp;
    EEG_Frame_t   eegFrame; //Only the Latest Frame, EEG Telemetry is managed by the C_EEG_IO class
    BRS_Frame_t   brsFrame;
    LED_Group_Fwd ledForward;
    LED_Group_Bwd ledBackward;
    LED_Group_Rgt ledRight;
    LED_Group_Lft ledLeft;
    ConnectionStatusType eegConnectionStatus;
    ConnectionStatusType pccConnectionStatus;
    ConnectionStatusType brsConnectionStatus;
    ConnectionStatusType flasherConnectionStatus;

    TM_Frame_t(EEG_Frame_t& eegFrame, BRS_Frame_t brsFrame)
        : TM_Frame_t()
    {
        this->eegFrame  = eegFrame;
        this->brsFrame  = brsFrame;
    }

    //Defaults
    TM_Frame_t()
    {
        timeStamp = 0;
        eegConnectionStatus     = NOT_CONNECTED;
        pccConnectionStatus     = NOT_CONNECTED;
        brsConnectionStatus     = NOT_CONNECTED;
        flasherConnectionStatus = NOT_CONNECTED;
    }

    TM_Frame_t(TM_Frame_t* other)
    {
        memcpy(this, other, sizeof(TM_Frame_t));
    }

    //Factory Constructors
    static TM_Frame_t* createFrame()
    {
        return new TM_Frame_t;
    }

    //Factory Constructors
    static TM_Frame_t* createFrame(TM_Frame_t* other)
    {
        return new TM_Frame_t(other);
    }

    static TM_Frame_t* createFrame(EEG_Frame_t& eegFrame, BRS_Frame_t brsFrame)
	{
        return new TM_Frame_t(eegFrame, brsFrame);
	}

    //destructor
    ~TM_Frame_t()
    {

        clear();
    }

    //Clear the current frame
    void clear()
    {
        memset(this, 0, sizeof(TM_Frame_t));
    }

} TM_Frame_t;

class C_TM
{
public:
     C_TM();
    ~C_TM();
	
	static C_TM* Instance() { return new C_TM; }
	
    void addFrame(TM_Frame_t* frame, int index=-1);//back of list by default
    TM_Frame_t* GetFrame(int index=-1);//last frame by default
    TM_Frame_t* popFrame();
    sizeType size(){ return (sizeType) tmFrames.size();}
	
    //Clear the TM Data
    void clear();

    C_TM& operator =(C_TM& rhs);
private:
    QList<TM_Frame_t*> tmFrames;
};

#endif // BCI_C_TM


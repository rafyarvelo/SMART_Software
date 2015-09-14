#ifndef BCI_C_TM
#define BCI_C_TM

#include <QList>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "../BRS/brs_c_abstract_sensor.h"
#include "../PCC/power_chair_command_constants.h"
#include "bci_c_eeg_data.h"
#include "bci_c_rvs.h"

//Data from the BRSH
typedef struct BRS_Frame_t
{
    sensorData_t     gpsData;
    sensorData_t     usData;
    PCC_Command_Type remoteCommand;

    BRS_Frame_t(sensorData_t& gpsData, sensorData_t& usData, PCC_Command_Type cmd)
    {
        this->gpsData       = gpsData;
        this->usData        = usData;
        this->remoteCommand = cmd;
    }

    BRS_Frame_t()
    {
        memset(&gpsData,0 , sizeof(sensorData_t));
        memset(&usData ,0 , sizeof(sensorData_t));
        remoteCommand = PCC_CMD_NONE;
    }

    static BRS_Frame_t* create(sensorData_t& gpsData, sensorData_t& usData, PCC_Command_Type cmd)
    {
        return new BRS_Frame_t(gpsData, usData, cmd);
    }

} BRS_Frame_t;

//A Single Frame of Telemetry Data
typedef struct TM_Frame_t
{
    EEG_Frame_t  eegFrame;
    BRS_Frame_t  brsFrame;
    LED_Group**  ledGroups;
    ConnectionStatusType eegConnectionStatus;
    ConnectionStatusType pccConnectionStatus;
    ConnectionStatusType brsConnectionStatus;
    ConnectionStatusType flasherConnectionStatus;

    TM_Frame_t(EEG_Frame_t& eegFrame, BRS_Frame_t brsFrame, LED_Group** ledGroups)
        : TM_Frame_t()
    {
        this->eegFrame  = eegFrame;
        this->brsFrame  = brsFrame;
        this->ledGroups = ledGroups;
    }

    //Defaults
    TM_Frame_t()
    {
        ledGroups = LED_Group::DefaultGroups();
        eegConnectionStatus     = NOT_CONNECTED;
        pccConnectionStatus     = NOT_CONNECTED;
        brsConnectionStatus     = NOT_CONNECTED;
        flasherConnectionStatus = NOT_CONNECTED;
    }

    //Factory Constructors
    static TM_Frame_t* createFrame()
    {
        return new TM_Frame_t;
    }

    static TM_Frame_t* createFrame(EEG_Frame_t& eegFrame, BRS_Frame_t brsFrame,
                                   LED_Group** ledGroups)
	{
        //Protect from Seg Faults
        if (!ledGroups)
        {
            ledGroups = LED_Group::DefaultGroups();
        }

        return new TM_Frame_t(eegFrame, brsFrame, ledGroups);
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
	 C_TM(){}//Implement later...lazy right now
	~C_TM(){}//Implement later...lazy right now
	
	static C_TM* Instance() { return new C_TM; }
	
    void addFrame(TM_Frame_t* tmFrame, int index=-1);//back of list by default
    TM_Frame_t* getFrame(int index=-1);//last frame by default
    TM_Frame_t* popFrame();
	
	sizeType size(){ return (sizeType) tmFrames.size();}
	
    QList<TM_Frame_t*> tmFrames;
};

#endif // BCI_C_TM


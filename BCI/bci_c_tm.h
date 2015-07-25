#ifndef BCI_C_TM
#define BCI_C_TM

#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "../BRS/brs_c_abstract_sensor.h"
#include "bci_c_eeg_io.h"
#include <vector>

typedef struct C_TM_Frame
{
	EEG_Data     eegData;
    sensorData_t GPS_Data;
    sensorData_t US_Data;

    C_TM_Frame(EEG_Data& eegData, sensorData_t& gpsData, sensorData_t& usData)
    {
    	this->eegData  = eegData;
    	this->GPS_Data = gpsData;
    	this->US_Data  = usData;
    }

	static C_TM_Frame* createFrame(EEG_Data& eegData, sensorData_t& gpsData, sensorData_t& usData)
	{
		return new C_TM_Frame(eegData, gpsData, usData);
	}
	
} C_TM_Frame;

class C_TM
{
public:
	 C_TM(){}//Implement later...lazy right now
	~C_TM(){}//Implement later...lazy right now
	
	static C_TM* Instance() { return new C_TM; }
	
	void addFrame(C_TM_Frame* tmFrame, int index=-1);//back of list by default
	C_TM_Frame* getFrame(int index=-1);//last frame by default
	C_TM_Frame* popFrame();
	
	sizeType size(){ return (sizeType) tmFrames.size();}
	
	vector<C_TM_Frame*> tmFrames; 
};

#endif // BCI_C_TM


#ifndef BCI_C_TM
#define BCI_C_TM

#include "../BRS/brs_c_abstract_sensor.h"
#include "bci_c_eeg_io.h"

typedef struct C_TM
{
	EEG_Data     eegData;
    sensorData_t GPS_Data;
    sensorData_t US_Data;
    unsigned int size;

    C_TM(EEG_Data eegData, sensorData_t gpsData, sensorData_t usData)
    {
    	this->eegData  = eegData;
    	this->GPS_Data = gpsData;
    	this->US_Data  = usData;
    	this->size     = eegData.size + gpsData.size + usData.size;
    }

} C_TM;

#endif // BCI_C_TM


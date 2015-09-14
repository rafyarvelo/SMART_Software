#ifndef EEG_IO_DEBUG
#define EEG_IO_DEBUG

#include <cstdlib>
#include <ctime>
#include "bci_c_eeg_io.h"

//Change As Needed...
#define MIN_DEBUG_DATA_SIZE   10
#define MAX_DEBUG_DATA_SIZE   10 

class C_EEG_IO_DEBUG : public C_EEG_IO
{
    Q_OBJECT
public:
             C_EEG_IO_DEBUG(){}
    virtual ~C_EEG_IO_DEBUG(){}
    static C_EEG_IO_DEBUG* Instance(){return new C_EEG_IO_DEBUG; }

    virtual C_EEG_Data&  getData() { return mDebugData; }
	virtual eegTypeEnum  getType() { return EEG_TYPE_DEBUG; }
    virtual ConnectionStatusType connect() {return CONNECTED;}

private:	
    C_EEG_Data& createRandomData(); //Creates Dummy Data
    C_EEG_Data mDebugData;
};

#endif


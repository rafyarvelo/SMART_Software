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
public:
			 C_EEG_IO_DEBUG();
	virtual ~C_EEG_IO_DEBUG();	
    static C_EEG_IO_DEBUG* Instance();

	virtual EEG_Data*    getData();
	virtual eegTypeEnum  getType() { return EEG_TYPE_DEBUG; }
    virtual ConnectionStatusType connect() {return CONNECTED;}

	void setDebugData(byteType* pData, sizeType size);
	void setDebugData(EEG_Data* pData);
    EEG_Data* createRandomData(); //Creates Dummy Data

private:	
	EEG_Data*        mDebugData;
};

#endif


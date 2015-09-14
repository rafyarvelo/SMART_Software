#ifndef EEG_IO_NAUTILUS
#define EEG_IO_NAUTILUS

#include "bci_c_eeg_io.h"

class C_EEG_IO_NAUTILUS : public C_EEG_IO
{
    Q_OBJECT
public:
			 C_EEG_IO_NAUTILUS(){}
	virtual ~C_EEG_IO_NAUTILUS(){}	
	static C_EEG_IO_NAUTILUS* Instance(){ return new C_EEG_IO_NAUTILUS; }
	
    virtual C_EEG_Data&  getData(){ return eegData; }
	virtual eegTypeEnum  getType(){ return EEG_TYPE_NAUTILUS; }
    virtual ConnectionStatusType connect(){return NOT_CONNECTED;}
private:
    C_EEG_Data eegData;
};

#endif

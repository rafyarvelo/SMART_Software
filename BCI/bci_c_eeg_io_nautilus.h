#ifndef EEG_IO_NAUTILUS
#define EEG_IO_NAUTILUS

#include "bci_c_eeg_io.h"

class C_EEG_IO_NAUTILUS : public C_EEG_IO, public C_Singleton<C_EEG_IO_NAUTILUS>
{
    Q_OBJECT
public:
			 C_EEG_IO_NAUTILUS(){}
    virtual ~C_EEG_IO_NAUTILUS(){}	
	
    virtual C_EEG_Data&  GetData(){ return eegData; }
    virtual eegTypeEnum  getType(){ return EEG_TYPE_NAUTILUS; }
    virtual ConnectionStatusType connect(){return NOT_CONNECTED;}

public slots:
    virtual bool fetchEEGFrame() { return true; }

private:
    C_EEG_Data eegData;
};

#endif

#ifndef EEG_IO_EMOTIV
#define EEG_IO_EMOTIV

#include "bci_c_eeg_io.h"

class C_EEG_IO_EMOTIV : public C_EEG_IO
{
public:
			 C_EEG_IO_EMOTIV(){}
	virtual ~C_EEG_IO_EMOTIV(){}
	static C_EEG_IO_EMOTIV* Instance() { return new C_EEG_IO_EMOTIV;}
	
	virtual EEG_Data*    getData() { return 0;}
	virtual eegTypeEnum  getType(){ return EEG_TYPE_EMOTIV; }
};

#endif

#ifndef BCI_C_SIGNAL_PROCESSING_H
#define BCI_C_SIGNAL_PROCESSING_H


#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_eeg_io.h"

class C_SignalProcessing
{
public:
	 C_SignalProcessing(C_EEG_IO* eegIO);
	~C_SignalProcessing();
	
	static  C_SignalProcessing* Instance(C_EEG_IO* eegIO)
	{
		return new C_SignalProcessing(eegIO);
	}
	
private:
	C_EEG_IO* mEEG_IO_Ptr;		
};

#endif // BCI_C_SIGNAL_PROCESSING_H


#include "bci_c_signal_processing.h"

C_SignalProcessing::C_SignalProcessing(C_EEG_IO* eegIO)
{
	mEEG_IO_Ptr = eegIO;
}

C_SignalProcessing::~C_SignalProcessing()
{
	
}

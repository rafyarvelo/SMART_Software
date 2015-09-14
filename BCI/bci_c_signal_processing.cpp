#include "bci_c_signal_processing.h"

C_SignalProcessing::C_SignalProcessing(C_EEG_IO* eegIO)
{
    mEEG_IO_Ptr      = eegIO;

    connect(mEEG_IO_Ptr, SIGNAL(EEGDataReady(C_EEG_Data&)),
            this       , SLOT(processEEGData(C_EEG_Data&)));
}

C_SignalProcessing::~C_SignalProcessing()
{
}

void C_SignalProcessing::processEEGData(C_EEG_Data &rawEEGData)
{
    //Do stuff here....
    processedData = rawEEGData;

    emit eegDataProcessed(processedData);
}

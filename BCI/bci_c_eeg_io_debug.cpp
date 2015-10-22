#include "bci_c_eeg_io_debug.h"

bool C_EEG_IO_DEBUG::fetchEEGFrame()
{
    EEG_Frame_t* frame = FrameGenerator::GenerateEEGFrame();

    //Get a new EEG Frame and Notify Listeners
    emit EEGFrameReceived(frame);

    if (recordTM)
    {
        eegTMFile->writeEEGFrame(frame);
    }

    return true; //Hell yea I got a bald head
}

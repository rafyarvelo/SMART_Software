#include "bci_c_eeg_io_debug.h"

bool C_EEG_IO_DEBUG::fetchEEGFrame()
{

    //Get a new EEG Frame and Signal that it's received
    eegData.AddFrame(FrameGenerator::GenerateEEGFrame());
    emit EEGFrameReceived(eegData.GetFramePtr());

    //Signal that our EEG Data is Ready every 10 Frames
    if (eegData.size() >= MIN_FRAMES_NEEDED)
    {
        emit EEGDataReady(eegData);
    }

    return true; //Hell yea I got a bald head
}

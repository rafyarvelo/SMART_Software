#include "bci_c_eeg_io_debug.h"

bool C_EEG_IO_DEBUG::fetchEEGFrame()
{
    //Generate a Random EEG Frame and Buffer it
    FrameGenerator::GenerateEEGFrame(&mCurrentEEGFrame);
    eegFrameBuffer.Put(mCurrentEEGFrame);

    //Notify Listeners that a frame is ready in the Buffer
    emit EEGFrameReceived(&eegFrameBuffer);

    if (recordTM)
    {
        eegTMFile->writeEEGFrame(&mCurrentEEGFrame);
    }

    return true; //Hell yea I got a bald head
}

#include "bci_c_eeg_data.h"

C_EEG_Data::C_EEG_Data(unsigned int bufferSize)
{
    //Store the EEG Frames in a semaphore protected Circular Buffer
    //with timeouts set to 5 ms
    frames = new C_SafeQueue<EEG_Frame_t*>(bufferSize,true, 5);
}

C_EEG_Data::~C_EEG_Data()
{
}

//Add a Frame if it is a valid pointer
void C_EEG_Data::AddFrame(EEG_Frame_t *eegFrame)
{
    if (eegFrame)
    {
        frames->Put(eegFrame);
    }
}

//Get the requested EEG Frame
EEG_Frame_t* C_EEG_Data::GetFramePtr(int index)
{
    static EEG_Frame_t* defaultFrame = createEEGFrame();

    if (frames->itemsAvailable() < 1)
    {
        return defaultFrame;
    }
    else
    {
        return frames->Get();
    }
}

//Overloaded Assignment Operator
C_EEG_Data& C_EEG_Data::operator =(C_EEG_Data& rhs)
{
    int size = rhs.frames->itemsAvailable();

    //Copy the EEG Data into our Current Object
    for (int i = 0; i < size; i++)
    {
        this->frames->Put(rhs.frames->Get());
    }

    return *this;
}
//===============================================

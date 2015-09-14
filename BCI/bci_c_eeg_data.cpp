#include "bci_c_eeg_data.h"

C_EEG_Data::C_EEG_Data(QObject *parent) :
    QObject(parent)
{
    currentFrame = 0;
    eegDataFull  = false;
}

C_EEG_Data::~C_EEG_Data()
{
}

//Add a Frame if we're not full and it is a valid pointer
void C_EEG_Data::AddFrame(EEG_Frame_t *eegFrame)
{
    if (currentFrame == MAX_EEG_FRAMES)
    {
        eegDataFull = true;
        emit EEG_Data_Full();
    }
    else
    {
        if (eegFrame)
        {
            frames.append(eegFrame);
            currentFrame++;
        }
    }
}

//Get the requested EEG Frame
EEG_Frame_t* C_EEG_Data::GetFramePtr(int index)
{
    if (index < 0 || index > currentFrame)
    {
        return frames.back();
    }
    else
    {
        return frames.at(index);
    }
}

//Get the requested EEG Frame
EEG_Frame_t& C_EEG_Data::GetFrame(int index)
{
    return *GetFramePtr(index);
}

//Clear all of the current frames
void C_EEG_Data::clear()
{
    frames.clear();
    currentFrame = 0;
    eegDataFull  = false;
}

C_EEG_Data& C_EEG_Data::operator =(C_EEG_Data& rhs)
{
    //Empty Current Data and replace it with this data
    clear();
    for (int i = 0; i < rhs.size(); i++)
    {
        frames.append(rhs.GetFramePtr(i));
    }
    currentFrame = frames.size();
    eegDataFull  = (currentFrame == MAX_EEG_FRAMES);
}


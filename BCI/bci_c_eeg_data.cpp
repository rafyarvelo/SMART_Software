#include "bci_c_eeg_data.h"

C_EEG_Data::C_EEG_Data(QObject* parent)
    : QObject(parent)
{
}

C_EEG_Data::~C_EEG_Data()
{
}

//Add a Frame if it is a valid pointer
void C_EEG_Data::AddFrame(EEG_Frame_t *eegFrame)
{
    //Empty Ourselves first if we're full
    if (frames.size() >= MAX_EEG_FRAMES)
    {
        emit EEG_Data_Full();
        clear();
    }

    if (eegFrame)
    {
        frames.append(eegFrame);
    }
}

//Get the requested EEG Frame
EEG_Frame_t* C_EEG_Data::GetFramePtr(int index)
{
    //Return a Blank Frame if we're Empty to avoid a seg fault
    if (frames.size() == 0)
    {
        return createEEGFrame();
    }

    //return last frame by default
    else if (index < 0 || index > frames.size())
    {
        return frames.back();
    }

    //return requested frame
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
    for (int i = 0; i < frames.size(); i++)
    {
        delete frames.at(i);
    }

    frames.clear();
}

//Overloaded Assignment Operator
C_EEG_Data& C_EEG_Data::operator =(C_EEG_Data& rhs)
{
    //Empty Current Data and replace it with this data
    clear();

    //Copy the EEG Data into our Current Object
    for (int i = 0; i < rhs.frames.size(); i++)
    {
        EEG_Frame_t* pFrame = createEEGFrame();
        memcpy(reinterpret_cast<void*>(pFrame), reinterpret_cast<const void*>(rhs.GetFramePtr(i)), sizeof(EEG_Frame_t));
        frames.append(pFrame);
    }

    return *this;
}
//===============================================

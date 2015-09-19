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
    if (currentFrame >= MAX_EEG_FRAMES)
    {
        eegDataFull = true;
        emit EEG_Data_Full();
        clear();
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
        //Return a Blank Frame if we're Empty to avoid a seg fault
        if (frames.size() == 0)
        {
            return EEG_Frame_t::create();
        }
        else
        {
            return frames.back();
        }
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
    for (int i = 0; i < frames.size(); i++)
    {
        delete frames.at(i);
    }

    frames.clear();
    currentFrame = 0;
    eegDataFull  = false;
}

//Overloaded Assignment Operator
C_EEG_Data& C_EEG_Data::operator =(C_EEG_Data& rhs)
{
    //Empty Current Data and replace it with this data
    clear();

    //Copy the EEG Data into our Current Object
    for (int i = 0; i < rhs.frames.size(); i++)
    {
        frames.append(EEG_Frame_t::create(rhs.frames.at(i)));
    }

    currentFrame = frames.size();
    eegDataFull  = (currentFrame == MAX_EEG_FRAMES);
}


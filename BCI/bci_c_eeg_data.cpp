#include "bci_c_eeg_data.h"

//===============EEG FRAME STRUCT================

//Default Frame
EEG_Frame_t::EEG_Frame_t()
{
    eegType = DEFAULT_EEG_TYPE;
    counter = 0;

    for (int i = 0; i < MAX_EEG_ELECTRODES;i++)
    {
        electrodeData[i]  = 0;
        contactQuality[i] = 0;
    }

    gyroX = 0;
    gyroY = 0;
    batteryPercentage = 0;
}

//Copy Constructor
EEG_Frame_t::EEG_Frame_t(EEG_Frame_t* other)
{
    memcpy(this, other, sizeof(EEG_Frame_t));
}

EEG_Frame_t* EEG_Frame_t::create(EEG_Frame_t* other)
{
    return new EEG_Frame_t(other);
}

EEG_Frame_t* EEG_Frame_t::create()
{
    return new EEG_Frame_t;
}

//Easy conversion from Emotiv since thats the default EEG
EEG_Frame_t* EEG_Frame_t::fromEmotivFrame(struct emokit_frame* emotiv_frame)
{
    EEG_Frame_t* frame = EEG_Frame_t::create();

    //Initialize frame from Emotiv Data
    frame->eegType = EEG_TYPE_EMOTIV;
    frame->counter = (quint32) emotiv_frame->counter;

    //Electrode Data from Emotiv Frame
    memcpy((void*) &frame->electrodeData[0], (void*) &emotiv_frame->F3,
            (size_t) sizeof(int) * NUM_EMOTIV_ELECTRODES);

    //Contact Quality from Emotiv Frame
    memcpy((void*) &frame->contactQuality[0], (void*) &emotiv_frame->cq,
            (size_t) sizeof(struct emokit_contact_quality));

    //The rest of the Emotiv Data
    frame->gyroX = emotiv_frame->gyroX;
    frame->gyroY = emotiv_frame->gyroY;
    frame->batteryPercentage = emotiv_frame->battery;

    return frame;
}
//===============================================

//===============EEG DATA CLASS==================
C_EEG_Data::C_EEG_Data(QObject *parent) :
    QObject(parent)
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
    if (index == 0 && frames.size() == 0)
    {
        return EEG_Frame_t::create();
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
        frames.append(EEG_Frame_t::create(rhs.frames.at(i)));
    }
}
//===============================================

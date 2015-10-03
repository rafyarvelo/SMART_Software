#include "bci_c_tm.h"

C_TM::C_TM()
{
}

C_TM::~C_TM()
{
}

void C_TM::addFrame(TM_Frame_t* frame, int index)
{
    //Don't Grow infinitely Large
    if (size() >= MAX_TM_FRAMES)
    {
        clear();
    }

    if (index < 0 || index > tmFrames.size())
    {
        tmFrames.append(frame);
    }
    else
    {
        tmFrames.insert(index, frame);
    }
}

void C_TM::clear()
{
    for (int i = 0; i < size(); i++)
    {
        delete tmFrames.at(i);
    }
    tmFrames.clear();
}

TM_Frame_t* C_TM::GetFrame(int index)
{
    if (index < 0 || index > tmFrames.size())
    {
        //Return the default frame if were empty to avoid a seg fault
        if (tmFrames.size() == 0)
        {
            return createTMFrame();
        }
        else
        {
            return tmFrames.back();
        }
    }
    else
    {
        return tmFrames.at(index);
    }
}

TM_Frame_t* C_TM::popFrame()
{
    return this->GetFrame();
}

C_TM& C_TM::operator =(C_TM& rhs)
{
    int i = 0;
    tmFrames.clear();

    //Copy Each Frame
    for (i = 0; i < rhs.size(); i++)
    {
        TM_Frame_t* pFrame = createTMFrame();
        memcpy(reinterpret_cast<void*>(pFrame),reinterpret_cast<const void*>(rhs.GetFrame(i)),sizeof(TM_Frame_t));
        tmFrames.append(pFrame);
    }
}

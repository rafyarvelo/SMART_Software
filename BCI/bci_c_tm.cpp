#include "bci_c_tm.h"

C_TM::C_TM()
{
}

C_TM::~C_TM()
{
}

void C_TM::addFrame(TM_Frame_t* frame, int index)
{
    if (index < 0 || index > tmFrames.size())
    {
        tmFrames.append(frame);
    }
    else
    {
        tmFrames.insert(index, frame);
    }
}

TM_Frame_t* C_TM::GetFrame(int index)
{
    if (index < 0 || index > tmFrames.size())
    {
        //Return the default frame if were empty to avoid a seg fault
        if (tmFrames.size() == 0)
        {
            return TM_Frame_t::createFrame();
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
    tmFrames.clear();

    for (int i = 0; i < rhs.size(); i++)
    {
        //We have access to private variable here
        tmFrames.append(TM_Frame_t::createFrame(rhs.tmFrames.at(i)));
    }
}

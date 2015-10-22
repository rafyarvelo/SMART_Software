#include "bci_c_tm.h"

C_TM::C_TM(unsigned int bufferSize)
{
    //Store the TM Frames in a Semaphore protected Circular FIFO
    //Array with timeouts set to 5ms
    tmFrames = new C_SafeQueue<TM_Frame_t*>(bufferSize, true, 5);
}

C_TM::~C_TM()
{
}

void C_TM::addFrame(TM_Frame_t* frame)
{
    if (frame)
    {
        tmFrames->Put(frame);
    }
}

//Get a Frame, return the latest if there is none available
TM_Frame_t* C_TM::GetFrame()
{
    static TM_Frame_t* defaultFrame = createTMFrame();

    if (tmFrames->itemsAvailable() < 1)
    {
        return defaultFrame;
    }
    else
    {
        return tmFrames->Get();
    }
}

C_TM& C_TM::operator =(C_TM& rhs)
{
    int size = tmFrames->itemsAvailable();

    //Copy Each Frame
    for (int i = 0; i < size; i++)
    {
        tmFrames->Put(rhs.tmFrames->Get());
    }

    return *this;
}

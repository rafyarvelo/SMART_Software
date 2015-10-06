#include "bci_c_brsh_io.h"

//Execute BRSH IO at 10 Hertz
const u_int16_t C_BRSH_IO::EXECUTION_RATE = 100;

//Create a Mutex to block the Current BRS Frame from Concurrent Access
QSemaphore* C_BRSH_IO::pBRSFrameMutex  = new QSemaphore(BRS_FRAME_MUTEX);

C_BRSH_IO::C_BRSH_IO()
{
    //Use the timer to control execution rate
    mTimer.setInterval(EXECUTION_RATE);
    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(fetchBRSFrame()));
    QObject::connect(&mThread, SIGNAL(started()), &mTimer, SLOT(start()));

    //Execute IO Tasks in a Seperate Thread
    QObject::moveToThread(&mThread);

    //Initialize the BRS Frame to NULL
    pLatestBRSFrame = 0;
}

C_BRSH_IO::~C_BRSH_IO()
{

}

BRS_Frame_t* C_BRSH_IO::GetLatestBRSFramePtr()
{
    //Try to Grab a Frame if someone requests it early
    if (!pLatestBRSFrame)
    {
        fetchBRSFrame();

        //if it's still 0, just make an empty one
        if (!pLatestBRSFrame)
        {
            return pLatestBRSFrame = createBRSFrame();
        }
        else
        {
            return pLatestBRSFrame;
        }
    }
    else
    {
        return pLatestBRSFrame;
    }
}

//Thread Execution of BRSH IO Class
void C_BRSH_IO::begin()
{
    mThread.start();
}

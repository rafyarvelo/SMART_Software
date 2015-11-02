#include "bci_c_brsh_io.h"

//Execute BRSH IO at 1 Hertz
const uint16_t C_BRSH_IO::EXECUTION_RATE = 1000;

C_BRSH_IO::C_BRSH_IO()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Use the timer to control execution rate
    mTimer.setInterval(EXECUTION_RATE);
    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(fetchBRSFrame()));
    QObject::connect(&mThread, SIGNAL(started()), &mTimer, SLOT(start()));

    //Execute IO Tasks in a Seperate Thread
    moveToThread(&mThread);
}

C_BRSH_IO::~C_BRSH_IO()
{

}

//Thread Execution of BRSH IO Class
void C_BRSH_IO::begin()
{
    mThread.start();
}

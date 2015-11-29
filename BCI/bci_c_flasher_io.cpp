#include "bci_c_flasher_io.h"

//Send RVS Sync every 1.05 Seconds 
const uint16_t C_Flasher_IO::EXECUTION_RATE = 1050;

C_Flasher_IO::C_Flasher_IO()
{
    //Setup Pointer for Log Files
    debugLog = SMART_DEBUG_LOG::Instance();
    
    //Use the timer to control execution rate
    mTimer.setInterval(EXECUTION_RATE);
    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(SendRVS()));
    QObject::connect(&mThread, SIGNAL(started()), &mTimer, SLOT(start()));

    //Execute IO Tasks in a Seperate Thread
    moveToThread(&mThread);
}

C_Flasher_IO::~C_Flasher_IO()
{	
}

void C_Flasher_IO::begin()
{
    mThread.start();
}

#include "bci_c_eeg_io.h"

//Execute the EEG IO Task at 5Hz
const uint16_t C_EEG_IO::EXECUTION_RATE = 200;

C_EEG_IO::C_EEG_IO()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Use the timer to control execution rate
    mTimer.setInterval(EXECUTION_RATE);
    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(fetchEEGFrame()));
    QObject::connect(&mThread, SIGNAL(started()), &mTimer, SLOT(start()));

    //Execute IO Tasks in a Seperate Thread
    moveToThread(&mThread);
}

C_EEG_IO::~C_EEG_IO()
{
}

//Thread Execution of EEG IO Class
void C_EEG_IO::begin()
{
    mThread.start();
}



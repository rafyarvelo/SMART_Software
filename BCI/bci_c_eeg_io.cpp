#include "bci_c_eeg_io.h"

//Execute the EEG IO Task at 50Hz
const u_int16_t C_EEG_IO::EXECUTION_RATE = 20;

C_EEG_IO::C_EEG_IO()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Use the timer to control execution rate
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(fetchEEGFrame()));
}

C_EEG_IO::~C_EEG_IO()
{

}

//Thread Execution of EEG IO Class
void C_EEG_IO::start()
{
    timer.start(EXECUTION_RATE);
}



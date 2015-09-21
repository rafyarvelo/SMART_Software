#include "bci_c_eeg_io.h"

//Execute the EEG IO Task at 50Hz
const u_int16_t C_EEG_IO_Task::EXECUTION_RATE = 20;

C_EEG_IO::C_EEG_IO()
{
    debugLog = SMART_DEBUG_LOG::Instance();
}

C_EEG_IO::~C_EEG_IO()
{

}

C_EEG_IO_Task::C_EEG_IO_Task(C_EEG_IO *pEEG_IO)
{
    mEEG_IO_Ptr = pEEG_IO;
}

//Singleton Creation of EEG IO Task Class
C_EEG_IO_Task* C_EEG_IO_Task::Instance(C_EEG_IO* pEEG_IO)
{
    static C_EEG_IO_Task* ptr = 0;

    if (!ptr)
    {
        ptr = new C_EEG_IO_Task(pEEG_IO);
    }

    return ptr;
}

//Thread Execution of EEG IO Class
void C_EEG_IO_Task::run()
{
    connect(&timer, SIGNAL(timeout()), mEEG_IO_Ptr, SLOT(fetchEEGFrame()));
    timer.start(EXECUTION_RATE);
    mEEG_IO_Ptr->moveToThread(this);
    exec();
}



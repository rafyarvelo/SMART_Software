#include "bci_c_brsh_io.h"

//Execute BRSH IO at 10 Hertz
const u_int16_t C_BRSH_IO_Task::EXECUTION_RATE = 100;

C_BRSH_IO::C_BRSH_IO()
{
    debugLog = SMART_DEBUG_LOG::Instance();
}

C_BRSH_IO::~C_BRSH_IO()
{

}

C_BRSH_IO_Task::C_BRSH_IO_Task(C_BRSH_IO* ptr)
{
    mBRSH_IO_Ptr = ptr;
}

//Execution of BRSH IO Thread
void C_BRSH_IO_Task::run()
{
    connect(&timer, SIGNAL(timeout()), mBRSH_IO_Ptr, SLOT(fetchBRSFrame()));
    timer.start(EXECUTION_RATE);
    mBRSH_IO_Ptr->moveToThread(this);
    exec();
}

//Singleton Creation of BRSH IO Task
C_BRSH_IO_Task* C_BRSH_IO_Task::Instance(C_BRSH_IO *pBRS_IO)
{
    static C_BRSH_IO_Task* ptr = 0;

    if (!ptr)
    {
        ptr = new C_BRSH_IO_Task(pBRS_IO);
    }

    return ptr;
}

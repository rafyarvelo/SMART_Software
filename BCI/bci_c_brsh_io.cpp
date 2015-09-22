#include "bci_c_brsh_io.h"

//Execute BRSH IO at 10 Hertz
const u_int16_t C_BRSH_IO::EXECUTION_RATE = 100;

C_BRSH_IO::C_BRSH_IO()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Use the timer to control execution rate
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(fetchBRSFrame()));
}

C_BRSH_IO::~C_BRSH_IO()
{

}

//Thread Execution of BRSH IO Class
void C_BRSH_IO::start()
{
    timer.start(EXECUTION_RATE);
}

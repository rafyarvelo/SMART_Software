#include "bci_c_brsh_io.h"

C_BRSH_IO::C_BRSH_IO()
    :tmDataBuffer(0)
{
    connectionStatus = CONNECTED;
}

C_BRSH_IO::~C_BRSH_IO()
{

}

void C_BRSH_IO::SendTM()
{

}

C_TM* C_BRSH_IO::GetLatestTM()
{
    return tmDataBuffer;
}

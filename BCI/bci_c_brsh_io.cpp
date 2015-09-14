#include "bci_c_brsh_io.h"

C_BRSH_IO::C_BRSH_IO()
{
    connectionStatus = CONNECTED;
}

C_BRSH_IO::~C_BRSH_IO()
{

}

void C_BRSH_IO::SendTMFrame()
{

}

void C_BRSH_IO::SetTMFrame(TM_Frame_t *pFrame)
{
    if (pFrame)
    {
        memcpy(&currentTMFrame, pFrame, sizeof(TM_Frame_t));
    }
}

BRS_Frame_t& C_BRSH_IO::GetLatestBRSFrame()
{
    return currentBRSFrame;
}

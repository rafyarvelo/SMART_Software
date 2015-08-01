#include "bci_c_flasher_io.h"

C_Flasher_IO::C_Flasher_IO(C_RVS* pRVS)
{
	mRVS_Ptr = pRVS;
    connectionStatus = CONNECTED;
}

C_Flasher_IO::~C_Flasher_IO()
{
	
}

void C_Flasher_IO::SendRVS()
{

}

#include "bci_c_flasher_io.h"

C_Flasher_IO::C_Flasher_IO()
{
}

C_Flasher_IO::~C_Flasher_IO()
{
	
}

ConnectionStatusType C_Flasher_IO::connect()
{
    connectionStatus = CONNECTED;
    return connectionStatus;
}

void C_Flasher_IO::SendRVS(C_RVS* pRVS)
{

}

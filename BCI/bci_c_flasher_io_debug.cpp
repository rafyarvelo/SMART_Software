#include "bci_c_flasher_io_debug.h"

C_Flasher_IO_Debug::C_Flasher_IO_Debug()
    : C_Flasher_IO()
{

}

C_Flasher_IO_Debug::~C_Flasher_IO_Debug()
{

}

ConnectionStatusType C_Flasher_IO_Debug::connect()
{
    connectionStatus = CONNECTED;
    return connectionStatus;
}

void C_Flasher_IO_Debug::SendRVS()
{
    debugLog->println(FLASHER_LOG, "Sending Repetitve Visual Stiumlus...", true);
}

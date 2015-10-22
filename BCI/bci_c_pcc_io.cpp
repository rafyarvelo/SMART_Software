#include "bci_c_pcc_io.h"

C_PCC_IO::C_PCC_IO()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    prevCommand    = PCC_CMD_NONE;
    currentCommand = PCC_CMD_NONE;
}

C_PCC_IO::~C_PCC_IO()
{

}

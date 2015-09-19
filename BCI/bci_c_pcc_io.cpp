#include "bci_c_pcc_io.h"

C_PCC_IO::C_PCC_IO()
{
}

C_PCC_IO::~C_PCC_IO()
{
}

void C_PCC_IO::SetCommand(PCC_Command_Type cmd)
{
    currentCommand = cmd;
}

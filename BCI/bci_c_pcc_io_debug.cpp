#include "bci_c_pcc_io_debug.h"

C_PCC_IO_Debug::C_PCC_IO_Debug()
{
}

void C_PCC_IO_Debug::SendCommand()
{
    if (currentCommand == prevCommand)
    {
        //Don't Send anything Here
    }
    else
    {
        QString toPrint = QString("Sending PCC Command: ") + QString(currentCommand);
        debugLog->println(PCC_LOG, toPrint.toStdString(), true);

        prevCommand = currentCommand;
    }
}

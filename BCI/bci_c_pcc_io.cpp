#include "bci_c_pcc_io.h"

C_PCC_IO::C_PCC_IO()
{
    debugLog   = SMART_DEBUG_LOG::Instance();
    serialComm = C_Serial_Comm::Instance(PCC_PORT);
}

C_PCC_IO::~C_PCC_IO()
{
    if (serialComm)
    {
        delete serialComm;
    }
}

void C_PCC_IO::SetCommand(PCC_Command_Type cmd)
{
    currentCommand = cmd;
}

void C_PCC_IO::SendCommand()
{
    QByteArray bytes;
    bytes.setRawData((const char*) &currentCommand, 1);

    debugLog->BCI_Log() << "Attempting to Send PCC Command: " << currentCommand << endl;
    serialComm->send(bytes);
}

ConnectionStatusType C_PCC_IO::connect()
{
    debugLog->BCI_Log() << "Attempting to Connect to Power Chair Controller..." << endl;
    if (serialComm->open())
    {
        debugLog->BCI_Log() << "Connected to Power Chair Controller!" << endl;
        connectionStatus = CONNECTED;
    }
    else
    {
        debugLog->BCI_Log() << "Could Not Connect to Power Chair Controller:(" << endl;
        connectionStatus = NOT_CONNECTED;
    }

    return connectionStatus;
}

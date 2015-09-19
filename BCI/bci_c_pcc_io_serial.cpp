#include "bci_c_pcc_io_serial.h"

C_PCC_IO_Serial::C_PCC_IO_Serial()
{
    debugLog   = SMART_DEBUG_LOG::Instance();
    serialComm = C_Serial_Comm::Instance(PCC_PORT);
    currentCommand = PCC_CMD_NONE;
}

C_PCC_IO_Serial::~C_PCC_IO_Serial()
{
    if (serialComm)
    {
        delete serialComm;
    }
}

void C_PCC_IO_Serial::SendCommand()
{
    QByteArray bytes;
    bytes.setRawData((const char*) &currentCommand, 1);

    debugLog->BCI_Log() << "Attempting to Send PCC Command: " << currentCommand << endl;
    serialComm->send(bytes);
}

ConnectionStatusType C_PCC_IO_Serial::connect()
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

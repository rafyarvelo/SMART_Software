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
    QString str = QString("Sending PCC Command: ") + QString(currentCommand);

    debugLog->println(PCC_LOG, str.toStdString(), true);
    serialComm->send(currentCommand);
}

ConnectionStatusType C_PCC_IO_Serial::connect()
{
    debugLog->println(PCC_LOG, "Attempting to Connect to Power Chair Controller...");

    if (serialComm->open())
    {
        debugLog->println(PCC_LOG, "Connected to Power Chair Controller!");
        connectionStatus = CONNECTED;
    }
    else
    {
        debugLog->println(PCC_LOG, "Could Not Connect to Power Chair Controller:(");
        connectionStatus = NOT_CONNECTED;
    }

    return connectionStatus;
}

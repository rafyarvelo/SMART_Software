#include "bci_c_pcc_io_serial.h"

C_PCC_IO_Serial::C_PCC_IO_Serial()
    : C_Serial_Comm(PCC_PORT)
{
    currentCommand = PCC_CMD_NONE;
}

C_PCC_IO_Serial::~C_PCC_IO_Serial()
{
}

void C_PCC_IO_Serial::SendCommand()
{
    QString str = QString("Sending PCC Command: ") + QString(currentCommand);

    debugLog->println(PCC_LOG, str.toStdString(), true);
    sendToSerialPort(currentCommand);
}

ConnectionStatusType C_PCC_IO_Serial::connect()
{
    debugLog->println(PCC_LOG, "Attempting to Connect to Power Chair Controller...");

    if (openSerialPort())
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

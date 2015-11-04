#include "bci_c_pcc_io_serial.h"
#include "bci_c_portnames.h"

C_PCC_IO_Serial::C_PCC_IO_Serial()
    : C_PCC_IO()
{
    C_PortNames* portNames = C_PortNames::Instance();
    QString portName = portNames->GetPCCPortName();

    mSerialPortPtr = new C_Serial_Comm(portName);
}

C_PCC_IO_Serial::~C_PCC_IO_Serial()
{
    delete mSerialPortPtr;
}

void C_PCC_IO_Serial::SendCommand(PCC_Command_Type cmd)
{
    QString toPrint;

    //Ignore Duplicate Commands
    if (cmd != prevCommand)
    {
        if (!mSerialPortPtr->Connected())
        {
            //Try to connect
            mSerialPortPtr->openSerialPort();
        }

        //Send the Command
        if (mSerialPortPtr->sendToSerialPort(cmd))
        {
            toPrint = QString("Sending PCC Command: ") + QString(cmd);
        }
        else
        {
            toPrint = QString("Couldn't Send Command: ") + QString(cmd);
        }
        debugLog->println(PCC_LOG, toPrint.toStdString(), true);
    }
}

void C_PCC_IO_Serial::EmergencyStop()
{
    PCC_Command_Type cmd = PCC_STOP;
    mSerialPortPtr->sendToSerialPort(cmd);
}

ConnectionStatusType C_PCC_IO_Serial::connect()
{
    debugLog->println(PCC_LOG, "Attempting to Connect to Power Chair Controller...");

    if (mSerialPortPtr->openSerialPort())
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

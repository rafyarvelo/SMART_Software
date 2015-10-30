#include "bci_c_pcc_io_serial.h"

C_PCC_IO_Serial::C_PCC_IO_Serial()
    : C_PCC_IO()
{
    //Set up the Execution Rate of the PCC IO Task
    mTimer.setInterval(PCC_COMMAND_SEND_RATE);
    QObject::connect(&mTimer, SIGNAL(timeout()),this, SLOT(onTimer()));
    QObject::connect(&mThread, SIGNAL(started()), &mTimer, SLOT(start()));

    //Store the PCC Commands in a Semaphore Protected Circular Buffer FIFO with
    //Timeouts Disabled
    pccCommandQueue = new C_SafeQueue<PCC_Command_Type>(PCC_COMMAND_BUFFER_SIZE, true, 0);

    mSerialPortPtr = new C_Serial_Comm(PCC_PORT);
}

C_PCC_IO_Serial::~C_PCC_IO_Serial()
{
}

void C_PCC_IO_Serial::SendCommand(PCC_Command_Type cmd)
{
    //Ignore Duplicate Commands
    if (cmd == prevCommand || pccCommandQueue->spacesAvailable() == 0)
    {
        return;
    }
    else
    {
        //Add the Command to our Queue, if we are full, the Calling thread will
        //be forced to wait on a semaphore
        pccCommandQueue->Put(cmd);
    }
}

//Try to Send the Curent Command to the Serial Port on the Timer Event
void C_PCC_IO_Serial::onTimer()
{
    PCC_Command_Type cmd = PCC_CMD_NONE;

    //Get the Next Command if there is one available
    if (pccCommandQueue->itemsAvailable() > 0)
    {
        cmd = pccCommandQueue->Get();

        if (cmd != prevCommand)
        {
            SendCmdToSerialPort(cmd);
        }
    }
    else
    {
        return;
    }
}

void C_PCC_IO_Serial::SendCmdToSerialPort(PCC_Command_Type cmd)
{
    char printBuffer[100];

    if (connectionStatus == CONNECTED)
    {
        //Try to Send the Command
        if (mSerialPortPtr->sendToSerialPort(cmd))
        {
            sprintf(printBuffer, "Sending PCC Command: %c", (char) cmd);
            debugLog->println(PCC_LOG, printBuffer, true);
        }
        else
        {
            sprintf(printBuffer, "Couldn't Send PCC Command: %c",(char) cmd);
            debugLog->println(PCC_LOG, printBuffer, true);
        }

        prevCommand = cmd;
    }
}

void C_PCC_IO_Serial::EmergencyStop()
{
    SendCmdToSerialPort(static_cast<PCC_Command_Type>(PCC_STOP));
}

ConnectionStatusType C_PCC_IO_Serial::connect()
{
    debugLog->println(PCC_LOG, "Attempting to Connect to Power Chair Controller...");

    if (mSerialPortPtr->openSerialPort())
    {
        debugLog->println(PCC_LOG, "Connected to Power Chair Controller!");
        connectionStatus = CONNECTED;
        mThread.start();
    }
    else
    {
        debugLog->println(PCC_LOG, "Could Not Connect to Power Chair Controller:(");
        connectionStatus = NOT_CONNECTED;
    }

    return connectionStatus;
}

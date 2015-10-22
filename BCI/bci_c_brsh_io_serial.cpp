#include "bci_c_brsh_io_serial.h"

C_BRSH_IO_Serial::C_BRSH_IO_Serial()
    : C_Serial_Comm(BRS_PORT)
{
    //Tiva C Uses a different Baud Rate
    mSerialPortPtr->setBaudRate(BAUD115200);
}

C_BRSH_IO_Serial::~C_BRSH_IO_Serial()
{

}

//Send TM to the BRSH
void C_BRSH_IO_Serial::SendTMFrame(TM_Frame_t* pFrame)
{
    //Write the TM Frame to the BRS through the UART Serial Port
    sendToSerialPort(reinterpret_cast<const TM_Frame_t*>(pFrame));

    //No longer need the Frame
    delete pFrame;
}

bool C_BRSH_IO_Serial::fetchBRSFrame()
{
    bool       received       = false;
    uint32_t   bytesAvailable = mSerialPortPtr->bytesAvailable();
    static int retryCount     = 0;
    BRS_Frame_t* frame        = 0;

    //Check if we're Connected
    if (!mSerialPortPtr->isOpen())
    {
        if (++retryCount > MAX_MISS_COUNT)
        {
            connectionStatus = false;
            mSerialPortPtr->close();
        }
        else
        {
            return false; //Don't Waste time just wait until the next fetch
        }
    }

    //Don't Bother if the data isn't there yet
    if (bytesAvailable < (sizeof(BRS_Frame_t)))
    {
        return false;
    }

    //Flush the Port if we're getting full
    else if (bytesAvailable >= MAX_BYTES_AVAILABLE)
    {
        mSerialPortPtr->readAll();
    }

    //Read in the Next BRS Message
    readFromSerialPort(frame);

    //Check if the Message was Formatted correctly
    if (checkMsgID(frame->MsgId, BRS2BCI_MSG_ID))
    {
        debugLogPtr->println(BRS_LOG, "MSG ID Received: BRS2BCI MSG ID\n");

        //Notify that it was received and save it in the buffer
        received = true;
        emit BRSFrameReceived(frame);
    }

    return received;
}

ConnectionStatusType C_BRSH_IO_Serial::connect()
{
    debugLogPtr->BRS_Log() << "Attempting to Connect to BRSH..." << endl;

    if (openSerialPort())
    {
        debugLogPtr->BRS_Log() << "Connected to BRSH!" << endl;
        connectionStatus = CONNECTED;
    }
    else
    {
        debugLogPtr->BCI_Log() << "Could Not Connect to BRSH:(" << endl;
        connectionStatus = NOT_CONNECTED;
    }

    return connectionStatus;
}
